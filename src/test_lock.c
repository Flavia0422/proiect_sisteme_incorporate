#include <avr/io.h>
#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"
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

#define LED     D12


#define SERVO_LOCKED    7   // 0°
#define SERVO_OPEN      20  // 90°


#define PASSWORD_LEN    4
const char PASSWORD[PASSWORD_LEN] = {'1', '2', '3', '4'};


typedef enum {
    STATE_LOCKED,
    STATE_OPEN
} safe_state_t;


void servo_init(void) {
    GPIO_Init(D9, GPIO_OUTPUT);
    PWM_Init(D9, 50);
    PWM_SetDutyCycle(D9, SERVO_LOCKED); //pornire in pozitia inchis 
}

void servo_lock(void) {
    PWM_SetDutyCycle(D9, SERVO_LOCKED);
}

void servo_open(void) {
    PWM_SetDutyCycle(D9, SERVO_OPEN);
}

//keypad-ul functioneaza pe un sistem de matrice 
//fiecare rand este conectat la un pin de output
//iar fiecare coloana la un pin de input cu pull-up

const char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init(void) {
    GPIO_Init(ROW1, GPIO_OUTPUT); GPIO_Write(ROW1, GPIO_HIGH);
    // Setam toate randurile ca output pentru ca apoi sa le putem seta LOW individual 
    GPIO_Init(ROW2, GPIO_OUTPUT); GPIO_Write(ROW2, GPIO_HIGH);
    GPIO_Init(ROW3, GPIO_OUTPUT); GPIO_Write(ROW3, GPIO_HIGH);
    GPIO_Init(ROW4, GPIO_OUTPUT); GPIO_Write(ROW4, GPIO_HIGH);

    GPIO_Init(COL1, GPIO_INPUT); GPIO_Write(COL1, GPIO_HIGH);
    //setam toate coloanele ca input pentru ca apoi sa putem citi starea lor 
    GPIO_Init(COL2, GPIO_INPUT); GPIO_Write(COL2, GPIO_HIGH);
    GPIO_Init(COL3, GPIO_INPUT); GPIO_Write(COL3, GPIO_HIGH);
    GPIO_Init(COL4, GPIO_INPUT); GPIO_Write(COL4, GPIO_HIGH);
}

char keypad_scan(void) {
    const uint8_t row_ports[] = {GPIO_PORTD, GPIO_PORTD, GPIO_PORTD, GPIO_PORTD};
    const uint8_t row_pins[]  = {2, 3, 4, 5};
    const uint8_t col_ports[] = {GPIO_PORTD, GPIO_PORTD, GPIO_PORTB, GPIO_PORTB};
    const uint8_t col_pins[]  = {6, 7, 0, 2};

    for (uint8_t r = 0; r < 4; r++) {
        GPIO_Write(row_ports[r], row_pins[r], GPIO_LOW);
        Delay(5);

        for (uint8_t c = 0; c < 4; c++) {
            if (GPIO_Read(col_ports[c], col_pins[c]) == GPIO_LOW) {
                while (GPIO_Read(col_ports[c], col_pins[c]) == GPIO_LOW);
                Delay(20);
                GPIO_Write(row_ports[r], row_pins[r], GPIO_HIGH);
                return keys[r][c];
            }
        }

        GPIO_Write(row_ports[r], row_pins[r], GPIO_HIGH);
    }

    return 0;
}

// feedback de la LED cand se apasa o tasta
void led_blink_keypress(void) {
    GPIO_Write(LED, GPIO_HIGH);
    Delay(80);
    GPIO_Write(LED, GPIO_LOW);
    Delay(80);
}
//comportament pentru LED cand parola introdus este gresita
void led_blink_wrong(void) {
    for (uint8_t i = 0; i < 3; i++) {
        GPIO_Write(LED, GPIO_HIGH);
        Delay(100);
        GPIO_Write(LED, GPIO_LOW);
        Delay(100);
    }
}


int main(void) {
    servo_init(); 
    keypad_init();
    GPIO_Init(LED, GPIO_OUTPUT);
    GPIO_Write(LED, GPIO_LOW);
    Delay(500);

    safe_state_t state = STATE_LOCKED;
    char input[PASSWORD_LEN];
    uint8_t input_pos = 0;

    while (1) {
        char key = keypad_scan();
        if (key == 0) continue;

        if (state == STATE_LOCKED) {
            if (key == '#' ) continue;

            led_blink_keypress();
            input[input_pos++] = key;

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
                    GPIO_Write(LED, GPIO_HIGH); 
                } else {
                    led_blink_wrong();
                    GPIO_Write(LED, GPIO_LOW);
                }

                input_pos = 0;
            }

        } else if (state == STATE_OPEN) {
            if (key == '#') {
                servo_lock();
                state = STATE_LOCKED;
                GPIO_Write(LED, GPIO_LOW);
                input_pos = 0;
            }
        }
    }
}