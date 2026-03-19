#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <utils/delay.h>


#define PAUZA_MISCARE 600 

void servo_init() {
    DDRB |= (1 << DDB1); // Pin D9 ieșire
    
    
}

void set_position(uint16_t pulse_us) {
    
    OCR1A ;
}

int main(void) {
    servo_init();
    
    // Așteptare inițială
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

    

    while (1) {
        // Stop
    }
}