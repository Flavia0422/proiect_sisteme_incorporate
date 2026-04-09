#include "utils.h"
#include "delay.h"
#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"
#include "bsp/nano.h"
#include "utils/delay.h"

int main(void) {
    PWM_Init(D9, 50);
    


    while(1) {
        PWM_SetDutyCycle(D9, 7); // 0
        Delay(500); 
        PWM_SetDutyCycle(D9, 20); // 90
        Delay(500);

        
    }
}