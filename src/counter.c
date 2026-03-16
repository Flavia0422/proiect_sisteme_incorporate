#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include "utils/delay.h"
#include "utils.h"

int main(void) {
    
    Timer0_Init();
    //GPIO_Init(LED_BUILTIN, GPIO_OUTPUT);

    GPIO_Init(D12, GPIO_OUTPUT);
    GPIO_Init(D11, GPIO_OUTPUT);
    GPIO_Init(D10, GPIO_OUTPUT);
    GPIO_Init(D8,GPIO_OUTPUT);

    uint32_t last_time = 0;
    uint8_t i = 0;

    while (1) {
        
       /*if (Millis() - last_time >= 1000) {
            last_time = Millis();
            GPIO_Toggle(LED_BUILTIN);
        }*/
        
        //problema counter

        GPIO_Write(D8, CHECK_BIT(i, 0));
        GPIO_Write(D10, CHECK_BIT(i, 1));
        GPIO_Write(D11, CHECK_BIT(i, 2));
        GPIO_Write(D12, CHECK_BIT(i, 3));
        
        i++;
        if (CHECK_BIT(i, 16)) {
            i = 0;
        }
        Delay(1000);
    }
}