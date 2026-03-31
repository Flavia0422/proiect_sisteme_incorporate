#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <utils/delay.h>

#define PAUZA_MISCARE 600 

void servo_init() {
    DDRB |= (1 << DDB1); // Pin D9 ieșire
    
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    ICR1 = 39999;
}

void set_position(uint16_t pulse_us) {
    OCR1A = (pulse_us * 2) - 1;
}

int main(void) {
    servo_init();
    
    Delay(1000);

    for (int i = 0; i < 3; i++) {
        // Poziția A (Maxim Stânga )
        set_position(700); 
        Delay(PAUZA_MISCARE); 

        // Poziția B (Maxim Dreapta )
        set_position(2300); 
        Delay(PAUZA_MISCARE); 
    }

    // Revenire la centru după finalizare
    set_position(1500);
    Delay(PAUZA_MISCARE);

    TCCR1A &= ~(1 << COM1A1); 

    while (1) {
        // Stop
    }
}
