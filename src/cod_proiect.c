#include <avr/io.h>
#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "drivers/pwm/pwm.h"
#include "drivers/display/display.h"
#include "bsp/nano.h"
#include "utils/delay.h"

//Keypad pins
#define ROW1    D2
#define ROW2    D3
#define ROW3    D4
#define ROW4    D5

#define COL1    D6
#define COL2    D7
#define COL3    D8
#define COL4    D10


#define BUZZER      A0
#define RED_LED     D12
#define GREEN_LED   A3


#define RGB_R   D11
#define RGB_G   A2
#define RGB_B   A1


#define SERVO_LOCKED    20// 90 degrees
#define SERVO_OPEN      7// 0 degrees


#define PASSWORD_LEN     4
#define MAX_ATTEMPTS     3
#define LOCKOUT_SECONDS  10


typedef enum {
    STATE_OPEN,
    STATE_SETUP,
    STATE_LOCKED,
    STATE_LOCKOUT
} safe_state_t;


char current_password[PASSWORD_LEN] = {0, 0, 0, 0};// stored in a vector so it can be changed in setup mode


const char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init(void) {
    GPIO_Init(ROW1, GPIO_OUTPUT); GPIO_Write(ROW1, GPIO_HIGH);
    GPIO_Init(ROW2, GPIO_OUTPUT); GPIO_Write(ROW2, GPIO_HIGH);
    GPIO_Init(ROW3, GPIO_OUTPUT); GPIO_Write(ROW3, GPIO_HIGH);
    GPIO_Init(ROW4, GPIO_OUTPUT); GPIO_Write(ROW4, GPIO_HIGH);

    GPIO_Init(COL1, GPIO_INPUT); GPIO_Write(COL1, GPIO_HIGH);
    GPIO_Init(COL2, GPIO_INPUT); GPIO_Write(COL2, GPIO_HIGH);
    GPIO_Init(COL3, GPIO_INPUT); GPIO_Write(COL3, GPIO_HIGH);
    GPIO_Init(COL4, GPIO_INPUT); GPIO_Write(COL4, GPIO_HIGH);
}

char keypad_scan(void) {
    // ROW1
    GPIO_Write(ROW1, GPIO_LOW); Delay(5);
    if (GPIO_Read(COL1) == GPIO_LOW) { while(GPIO_Read(COL1) == GPIO_LOW); Delay(20); GPIO_Write(ROW1, GPIO_HIGH); return keys[0][0]; }
    if (GPIO_Read(COL2) == GPIO_LOW) { while(GPIO_Read(COL2) == GPIO_LOW); Delay(20); GPIO_Write(ROW1, GPIO_HIGH); return keys[0][1]; }
    if (GPIO_Read(COL3) == GPIO_LOW) { while(GPIO_Read(COL3) == GPIO_LOW); Delay(20); GPIO_Write(ROW1, GPIO_HIGH); return keys[0][2]; }
    if (GPIO_Read(COL4) == GPIO_LOW) { while(GPIO_Read(COL4) == GPIO_LOW); Delay(20); GPIO_Write(ROW1, GPIO_HIGH); return keys[0][3]; }
    GPIO_Write(ROW1, GPIO_HIGH);

    // ROW2
    GPIO_Write(ROW2, GPIO_LOW); Delay(5);
    if (GPIO_Read(COL1) == GPIO_LOW) { while(GPIO_Read(COL1) == GPIO_LOW); Delay(20); GPIO_Write(ROW2, GPIO_HIGH); return keys[1][0]; }
    if (GPIO_Read(COL2) == GPIO_LOW) { while(GPIO_Read(COL2) == GPIO_LOW); Delay(20); GPIO_Write(ROW2, GPIO_HIGH); return keys[1][1]; }
    if (GPIO_Read(COL3) == GPIO_LOW) { while(GPIO_Read(COL3) == GPIO_LOW); Delay(20); GPIO_Write(ROW2, GPIO_HIGH); return keys[1][2]; }
    if (GPIO_Read(COL4) == GPIO_LOW) { while(GPIO_Read(COL4) == GPIO_LOW); Delay(20); GPIO_Write(ROW2, GPIO_HIGH); return keys[1][3]; }
    GPIO_Write(ROW2, GPIO_HIGH);

    // ROW3
    GPIO_Write(ROW3, GPIO_LOW); Delay(5);
    if (GPIO_Read(COL1) == GPIO_LOW) { while(GPIO_Read(COL1) == GPIO_LOW); Delay(20); GPIO_Write(ROW3, GPIO_HIGH); return keys[2][0]; }
    if (GPIO_Read(COL2) == GPIO_LOW) { while(GPIO_Read(COL2) == GPIO_LOW); Delay(20); GPIO_Write(ROW3, GPIO_HIGH); return keys[2][1]; }
    if (GPIO_Read(COL3) == GPIO_LOW) { while(GPIO_Read(COL3) == GPIO_LOW); Delay(20); GPIO_Write(ROW3, GPIO_HIGH); return keys[2][2]; }
    if (GPIO_Read(COL4) == GPIO_LOW) { while(GPIO_Read(COL4) == GPIO_LOW); Delay(20); GPIO_Write(ROW3, GPIO_HIGH); return keys[2][3]; }
    GPIO_Write(ROW3, GPIO_HIGH);

    // ROW4
    GPIO_Write(ROW4, GPIO_LOW); Delay(5);
    if (GPIO_Read(COL1) == GPIO_LOW) { while(GPIO_Read(COL1) == GPIO_LOW); Delay(20); GPIO_Write(ROW4, GPIO_HIGH); return keys[3][0]; }
    if (GPIO_Read(COL2) == GPIO_LOW) { while(GPIO_Read(COL2) == GPIO_LOW); Delay(20); GPIO_Write(ROW4, GPIO_HIGH); return keys[3][1]; }
    if (GPIO_Read(COL3) == GPIO_LOW) { while(GPIO_Read(COL3) == GPIO_LOW); Delay(20); GPIO_Write(ROW4, GPIO_HIGH); return keys[3][2]; }
    if (GPIO_Read(COL4) == GPIO_LOW) { while(GPIO_Read(COL4) == GPIO_LOW); Delay(20); GPIO_Write(ROW4, GPIO_HIGH); return keys[3][3]; }
    GPIO_Write(ROW4, GPIO_HIGH);

    return 0;
}


void servo_init(void) {
    GPIO_Init(D9, GPIO_OUTPUT);
    PWM_Init(D9, 50);
    PWM_SetDutyCycle(D9, SERVO_OPEN);
}

void servo_lock(void) { PWM_SetDutyCycle(D9, SERVO_LOCKED); }
void servo_open(void) { PWM_SetDutyCycle(D9, SERVO_OPEN); }


void rgb_set(uint8_t r, uint8_t g, uint8_t b) {
    GPIO_Write(RGB_R, r ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(RGB_G, g ? GPIO_HIGH : GPIO_LOW);
    GPIO_Write(RGB_B, b ? GPIO_HIGH : GPIO_LOW);
}

void rgb_off(void) { rgb_set(0, 0, 0); }


void buzzer_keypress(void) {
    GPIO_Write(BUZZER, GPIO_HIGH); Delay(30);
    GPIO_Write(BUZZER, GPIO_LOW);
}

void buzzer_correct(void) {
    GPIO_Write(BUZZER, GPIO_HIGH); Delay(200);
    GPIO_Write(BUZZER, GPIO_LOW);  Delay(100);
    GPIO_Write(BUZZER, GPIO_HIGH); Delay(400);
    GPIO_Write(BUZZER, GPIO_LOW);
}

void buzzer_wrong(void) {
    GPIO_Write(BUZZER, GPIO_HIGH); Delay(50);
    GPIO_Write(BUZZER, GPIO_LOW);  Delay(50);
    GPIO_Write(BUZZER, GPIO_HIGH); Delay(50);
    GPIO_Write(BUZZER, GPIO_LOW);  Delay(200);
    GPIO_Write(BUZZER, GPIO_HIGH); Delay(300);
    GPIO_Write(BUZZER, GPIO_LOW);
}

void buzzer_alarm(void) {
    for (uint8_t i = 0; i < 10; i++) {
        GPIO_Write(BUZZER, GPIO_HIGH); Delay(100);
        GPIO_Write(BUZZER, GPIO_LOW);  Delay(100);
    }
}


void display_open(void) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(22, 3, "UNLOCKED");
    Display_Print(0,  5, "Press * to set PIN");
    Display_Update();
}

void display_setup(uint8_t digits) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(4,  2, "Set your PIN:");
    Display_Print(0,  4, "Enter 4 digits");
    Display_Print(0,  5, "then press *");
    char stars[5] = "    ";
    for (uint8_t i = 0; i < digits; i++) stars[i] = '*';
    Display_Print(0, 7, stars);
    Display_Update();
}

void display_locked(uint8_t attempts) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(28, 2, "LOCKED");
    Display_Print(0,  4, "Enter PIN(4 digits):");
    char attempts_str[17] = "Attempts left: ";
    attempts_str[15] = '0' + (MAX_ATTEMPTS - attempts);
    attempts_str[16] = '\0';
    Display_Print(0, 6, attempts_str);
    Display_Update();
}

void display_typing(uint8_t digits, uint8_t attempts) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(28, 2, "LOCKED");
    Display_Print(0,  4, "Enter PIN(4 digits):");
    char stars[5] = "    ";
    for (uint8_t i = 0; i < digits; i++) stars[i] = '*';
    Display_Print(0, 6, stars);
    char attempts_str[10] = "Tries: ";
    attempts_str[7] = '0' + (MAX_ATTEMPTS - attempts);
    attempts_str[8] = '\0';
    Display_Print(64, 6, attempts_str);
    Display_Update();
}

void display_granted(void) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(10, 3, "ACCESS GRANTED");
    Display_Update();
}

void display_denied(uint8_t attempts) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(16, 3, "ACCESS DENIED");
    char attempts_str[17] = "Attempts left: ";
    attempts_str[15] = '0' + (MAX_ATTEMPTS - attempts);
    attempts_str[16] = '\0';
    Display_Print(0, 5, attempts_str);
    Display_Update();
}

void display_lockout(uint8_t seconds) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(4,  2, "ALL ATTEMPTS FAILED!");
    Display_Print(0,  4, "Try again in:");
    char countdown[10] = "          ";
    if (seconds >= 10) {
        countdown[0] = '0' + (seconds / 10);
        countdown[1] = '0' + (seconds % 10);
        countdown[2] = 's';
        countdown[3] = '\0';
    } else {
        countdown[0] = '0' + seconds;
        countdown[1] = 's';
        countdown[2] = '\0';
    }
    Display_Print(0, 6, countdown);
    Display_Update();
}

void display_pin_set(void) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(16, 3, "PIN SET!");
    Display_Print(4,  5, "Safe is locking...");
    Display_Update();
}


int main(void) {
    servo_init();
    keypad_init();
    Display_Init();

    GPIO_Init(BUZZER,    GPIO_OUTPUT); GPIO_Write(BUZZER,    GPIO_LOW);
    GPIO_Init(RED_LED,   GPIO_OUTPUT); GPIO_Write(RED_LED,   GPIO_LOW);
    GPIO_Init(GREEN_LED, GPIO_OUTPUT); GPIO_Write(GREEN_LED, GPIO_LOW);
    GPIO_Init(RGB_R,     GPIO_OUTPUT); GPIO_Write(RGB_R,     GPIO_LOW);
    GPIO_Init(RGB_G,     GPIO_OUTPUT); GPIO_Write(RGB_G,     GPIO_LOW);
    GPIO_Init(RGB_B,     GPIO_OUTPUT); GPIO_Write(RGB_B,     GPIO_LOW);

    // Boot: open, green LED on
    servo_open();
    GPIO_Write(GREEN_LED, GPIO_HIGH);
    display_open();

    safe_state_t state = STATE_OPEN;
    char input[PASSWORD_LEN];
    uint8_t input_pos = 0;
    uint8_t attempts  = 0;

    const uint8_t colors[][3] = {
        {1, 0, 0},
        {1, 1, 0},
        {0, 1, 0},
        {0, 1, 1},
        {0, 0, 1},
        {1, 0, 1},
    };
    uint8_t color_count = 6;

    while (1) {

        
        if (state == STATE_LOCKOUT) {
            buzzer_alarm();
            uint8_t color_idx = 0;

            for (uint8_t s = LOCKOUT_SECONDS; s > 0; s--) {
                for (uint8_t step = 0; step < 3; step++) {
                    rgb_set(colors[color_idx][0], colors[color_idx][1], colors[color_idx][2]);
                    color_idx = (color_idx + 1) % color_count;
                    display_lockout(s);
                    Delay(150);
                }
            }

            rgb_off();
            attempts  = 0;
            input_pos = 0;
            state     = STATE_LOCKED;
            display_locked(attempts);
            continue;
        }

        char key = keypad_scan();
        if (key == 0) continue;

        
        if (state == STATE_OPEN) {
            if (key == '*') {
                input_pos = 0;
                state = STATE_SETUP;
                display_setup(0);
            }
        }

        
        else if (state == STATE_SETUP) {
            if (key == '#') continue;

            if (key == '*') {
                if (input_pos == PASSWORD_LEN) {
                    for (uint8_t i = 0; i < PASSWORD_LEN; i++) {
                        current_password[i] = input[i];
                    }
                    input_pos = 0;
                    attempts  = 0;
                    buzzer_correct();
                    display_pin_set();
                    Delay(500);
                    servo_lock();
                    GPIO_Write(GREEN_LED, GPIO_LOW);
                    state = STATE_LOCKED;
                    display_locked(attempts);
                }
                continue;
            }

            if (input_pos < PASSWORD_LEN) {
                buzzer_keypress();
                input[input_pos++] = key;
                display_setup(input_pos);
            }
        }

        
        else if (state == STATE_LOCKED) {
            if (key == '#' || key == '*') continue;

            buzzer_keypress();
            input[input_pos++] = key;
            display_typing(input_pos, attempts);

            if (input_pos >= PASSWORD_LEN) {
                uint8_t correct = 1;
                for (uint8_t i = 0; i < PASSWORD_LEN; i++) {
                    if (input[i] != current_password[i]) {
                        correct = 0;
                        break;
                    }
                }

                if (correct) {
                    attempts = 0;
                    servo_open();
                    state = STATE_OPEN;
                    GPIO_Write(GREEN_LED, GPIO_HIGH);
                    buzzer_correct();
                    display_granted();
                    Delay(1500);
                    display_open();
                } else {
                    attempts++;
                    GPIO_Write(RED_LED, GPIO_HIGH);
                    buzzer_wrong();
                    display_denied(attempts);
                    Delay(500);
                    GPIO_Write(RED_LED, GPIO_LOW);

                    if (attempts >= MAX_ATTEMPTS) {
                        state = STATE_LOCKOUT;
                    } else {
                        display_locked(attempts);
                    }
                }

                input_pos = 0;
            }
        }
    }
}