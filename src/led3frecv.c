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
            
    //prima cerinta: led cu 3 frecvente diferite
for( i=0 ;i<10;i++){
    GPIO_Write(D12,GPIO_HIGH);
    Delay(50);
    GPIO_Write(D12,GPIO_LOW);
    Delay(50);
    }
Delay(500);
for( i=0 ;i<10;i++){
    GPIO_Write(D12,GPIO_HIGH);
    Delay(150);
    GPIO_Write(D12,GPIO_LOW);
    Delay(150);
    }
Delay(500);
for( i=0 ;i<10;i++){
    GPIO_Write(D12,GPIO_HIGH);
    Delay(300);
    GPIO_Write(D12,GPIO_LOW);
    Delay(300);
    }
    Delay(500);



    }
}