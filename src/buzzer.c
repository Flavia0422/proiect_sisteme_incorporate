
#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include "utils/delay.h"

int main(void) {
    
    GPIO_Init(A0, GPIO_OUTPUT);
    GPIO_Write(A0, GPIO_LOW);

    while (1) {
        
        GPIO_Write(A0, GPIO_HIGH);
        Delay(100);
        
        GPIO_Write(A0, GPIO_LOW);
        Delay(500);
    }
}