#include <avr/io.h>
#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "drivers/pwm/pwm.h"
#include "drivers/display/display.h"
#include "bsp/nano.h"
#include "utils/delay.h"


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


#define SERVO_LOCKED    20
#define SERVO_OPEN      7


#define PASSWORD_LEN    4
const char PASSWORD[PASSWORD_LEN] = {'1', '2', '3', '4'};


typedef enum {
    STATE_LOCKED,
    STATE_OPEN
} safe_state_t;


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
    PWM_SetDutyCycle(D9, SERVO_LOCKED);
}

void servo_lock(void) { PWM_SetDutyCycle(D9, SERVO_LOCKED); }
void servo_open(void) { PWM_SetDutyCycle(D9, SERVO_OPEN); }


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

    GPIO_Write(BUZZER, GPIO_HIGH); Delay(50);
    GPIO_Write(BUZZER, GPIO_LOW);  Delay(50);
    GPIO_Write(BUZZER, GPIO_HIGH); Delay(50);
    GPIO_Write(BUZZER, GPIO_LOW);  Delay(200);

    GPIO_Write(BUZZER, GPIO_HIGH); Delay(600);
    GPIO_Write(BUZZER, GPIO_LOW);
}


void display_locked(void) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(28, 2, "LOCKED");
    Display_Print(0,  4, "Enter PIN:");
    Display_Update();
}

void display_typing(uint8_t digits) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(28, 2, "LOCKED");
    Display_Print(0,  4, "Enter PIN:");
    char stars[5] = "    ";
    for (uint8_t i = 0; i < digits; i++) stars[i] = '*';
    Display_Print(0, 6, stars);
    Display_Update();
}

void display_granted(void) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(10, 3, "ACCESS GRANTED");
    Display_Print(4,  5, "# to lock again");
    Display_Update();
}

void display_denied(void) {
    Display_Clear();
    Display_Print(16, 0, "** SAFE BOX **");
    Display_Print(16, 3, "ACCESS DENIED");
    Display_Print(10, 5, "Try again...");
    Display_Update();
}


int main(void) {
   
    servo_init();
    keypad_init();
    Display_Init();

    GPIO_Init(BUZZER,    GPIO_OUTPUT); GPIO_Write(BUZZER,    GPIO_LOW);
    GPIO_Init(RED_LED,   GPIO_OUTPUT); GPIO_Write(RED_LED,   GPIO_LOW);
    GPIO_Init(GREEN_LED, GPIO_OUTPUT); GPIO_Write(GREEN_LED, GPIO_LOW);

    
    servo_lock();
    display_locked();
    Delay(500);

    safe_state_t state = STATE_LOCKED;
    char input[PASSWORD_LEN];
    uint8_t input_pos = 0;

    while (1) {
        char key = keypad_scan();
        if (key == 0) continue;

        if (state == STATE_LOCKED) {
            if (key == '#' || key == '*') continue;

            
            buzzer_keypress();
            input[input_pos++] = key;
            display_typing(input_pos);

            if (input_pos >= PASSWORD_LEN) {
                
                uint8_t correct = 1;
                for (uint8_t i = 0; i < PASSWORD_LEN; i++) {
                    if (input[i] != PASSWORD[i]) {
                        correct = 0;
                        break;
                    }
                }

                if (correct) {
                    
                    servo_open();
                    state = STATE_OPEN;
                    GPIO_Write(GREEN_LED, GPIO_HIGH);
                    buzzer_correct();
                    display_granted();
                } else {
                    
                    GPIO_Write(RED_LED, GPIO_HIGH);
                    buzzer_wrong();
                    display_denied();
                    Delay(2000);
                    GPIO_Write(RED_LED, GPIO_LOW);
                    display_locked();
                }

                input_pos = 0;
            }

        } else if (state == STATE_OPEN) {
            if (key == '#') {
                servo_lock();
                state = STATE_LOCKED;
                GPIO_Write(GREEN_LED, GPIO_LOW);
                buzzer_keypress();
                display_locked();
                input_pos = 0;
            }
        }
    }
}