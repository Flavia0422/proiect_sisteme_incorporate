#include "utils.h"
#include "delay.h"
#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"
#include "bsp/nano.h"
#include "utils/delay.h"
int main(void) {
    PWM_Init(GPIO_PORTB, 1, 50);

    uint8_t duty = 13; // 
    int8_t step = 2;

    while(1) {
        PWM_SetDutyCycle(GPIO_PORTB, 1, duty);
        Delay(500); 

        duty += step;

        if (duty >= 26) { // 180
            step = -2;
        } else if (duty <= 13) { // 0
            step = 2;
        }
    }
}
