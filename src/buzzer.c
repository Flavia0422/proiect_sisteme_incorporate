#include "drivers/gpio/gpio.h"
#include "bsp/nano.h"
#include "utils/delay.h"

#define BUZZER  A0

//TESTARE SUNETE BUZZER
void buzzer(void) {
    
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

int main(void) {
    GPIO_Init(BUZZER, GPIO_OUTPUT);
    GPIO_Write(BUZZER, GPIO_LOW);

    while (1) {
        buzzer();
        Delay(2000);
    }
}