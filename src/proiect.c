#include <avr/io.h>
#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include "utils/delay.h"
//TESTARE SENZOR DISTANTA HC-SR04 
//CAND APROPII CEVA DE SENZOR SE APRINDE UN LED
//DACA NU MAI E NIMIC APROAPE LEDUL SE STINGE

#define TRIG    D3
#define ECHO    D4
#define LED     D7

#define DISTANCE_THRESHOLD_CM   20

//resetare timere ca sa nu mai interfereze cu alti pini
void timers_reset(void) {
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    ICR1   = 0;
    OCR1A  = 0;
    TIMSK1 = 0;

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;
    OCR2A  = 0;
    TIMSK2 = 0;

    DDRB &= ~(1 << 1);  // D9  
    DDRB &= ~(1 << 3);  // D11 
}

// senzorul de distanta HC-SR04 foloseste un singur pin pentru trigger si echo, 
//dar in modul digital GPIO trebuie sa-l configuram separat ca input si output.
void hcsr04_init(void) {
    GPIO_Init(TRIG, GPIO_OUTPUT);
    GPIO_Init(ECHO, GPIO_INPUT);
    GPIO_Write(TRIG, GPIO_LOW);
}

uint16_t hcsr04_read_cm(void) {
    
    GPIO_Write(TRIG, GPIO_HIGH);
    for (volatile uint8_t i = 0; i < 160; i++) __asm volatile("nop");
    GPIO_Write(TRIG, GPIO_LOW);

    
    uint32_t t_start = Millis();
    while (GPIO_Read(ECHO) == GPIO_LOW) {
        if (Millis() - t_start > 10) return 9999;
    }

    
    uint32_t t_echo_start = Millis();
    while (GPIO_Read(ECHO) == GPIO_HIGH) {
        if (Millis() - t_echo_start > 30) return 9999;
    }
    uint32_t duration_ms = Millis() - t_echo_start;

    //distanta = durata_ms * 17
    // sunetul calatoreste 34cm/ms, dar trebuie sa impartim la 2 pentru ca e dus-intors.
    return (uint16_t)(duration_ms * 17);
}


int main(void) {
    timers_reset();
    Timer0_Init();
    hcsr04_init();
    GPIO_Init(LED, GPIO_OUTPUT);

    while (1) {
        uint16_t dist = hcsr04_read_cm();

        if (dist == 9999) {
            // Sensor error — fast blink
            GPIO_Write(LED, GPIO_HIGH);
            Delay(100);
            GPIO_Write(LED, GPIO_LOW);
            Delay(100);
        } else if (dist < DISTANCE_THRESHOLD_CM) {
            // Object detected — LED solid ON
            GPIO_Write(LED, GPIO_HIGH);
        } else {
            // No object — LED solid OFF
            GPIO_Write(LED, GPIO_LOW);
        }

        Delay(100);
    }
}