#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include "utils/delay.h"

int main(void) {
    
    Timer0_Init();
    //GPIO_Init(LED_BUILTIN, GPIO_OUTPUT);

    GPIO_Init(D12, GPIO_OUTPUT);
     GPIO_Init(D11, GPIO_OUTPUT);
      GPIO_Init(D10, GPIO_OUTPUT);


    uint32_t last_time = 0;
    uint8_t i;

    while (1) {
            
       /*if (Millis() - last_time >= 1000) {
            last_time = Millis();
            GPIO_Toggle(LED_BUILTIN);
        }*/
        
        //problema semafor
        
         GPIO_Write(D12,GPIO_HIGH);
         Delay(2000);
         GPIO_Write(D11,GPIO_HIGH);
         Delay(100);
         GPIO_Write(D12,GPIO_LOW);
         GPIO_Write(D11,GPIO_LOW);
        
 
         GPIO_Write(D11,GPIO_HIGH);
         Delay(300);
        GPIO_Write(D11,GPIO_LOW);

         GPIO_Write(D10,GPIO_HIGH);
         Delay(1500);
         GPIO_Write(D10,GPIO_LOW);

        GPIO_Write(D11,GPIO_HIGH);
         Delay(150);
        GPIO_Write(D11,GPIO_LOW);

    }
}