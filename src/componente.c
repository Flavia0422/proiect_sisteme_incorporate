#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include "utils/delay.h"
#include <avr/io.h>

#define TRIG_PIN D3
#define ECHO_PIN D4
#define LED_PIN D11
#define DISTANCE_THRESHOLD 20

unsigned long pulseIn(gpio_port_t port, uint8_t pin, gpio_state_t state) {
    unsigned long counter = 0; // pentru masurarea duratei pulsului
    unsigned long timeout = 400000;// Timeout pentru a preveni blocarea indefinita

    while (GPIO_Read(port, pin) == state && timeout > 0) {
        // Asteapta pana cand pinul atinge starea opusa
        timeout--;// Decrementam timeout-ul pentru a preveni blocarea indefinita
    }

    
    while (GPIO_Read(port, pin) != state && timeout > 0) {
        // Asteapta pana cand pinul atinge starea dorita
        timeout--;
    }

     
    while (GPIO_Read(port, pin) == state && timeout > 0) {
        // Masuram durata pulsului
        counter++;// Incrementam counter-ul pentru a masura durata pulsului
        timeout--;//    Decrementam timeout-ul pentru a preveni blocarea indefinita
    }

    return counter; // Returnam durata pulsului pentru calculul distantei
}

int main(void) {
   
    GPIO_Init(TRIG_PIN, GPIO_OUTPUT);
    GPIO_Init(ECHO_PIN, GPIO_INPUT);
    GPIO_Init(LED_PIN, GPIO_OUTPUT);

    unsigned long pulse_duration;// pentru a stoca durata pulsului(semnalul de la senzor)
    unsigned int distance_cm;

    while (1) {
       
        GPIO_Write(TRIG_PIN, GPIO_LOW);// Asiguram ca pinul de trigger este initial in starea LOW
        Delay(2);
        GPIO_Write(TRIG_PIN, GPIO_HIGH);// Generam un puls de 10 microsecunde pentru a declansa masuratoarea
        Delay(1);  
        GPIO_Write(TRIG_PIN, GPIO_LOW);// Oprirea pulsului

        
        pulse_duration = pulseIn(ECHO_PIN, GPIO_HIGH);// Masuram durata pulsului de la senzor

        
        distance_cm = (pulse_duration >> 4) * 0.034 / 2; 
        // Calculam distanta in centimetri folosind formula:
        // distanta = (durata_puls * viteza_sunet) / 2 


        if (distance_cm > 0 && distance_cm < DISTANCE_THRESHOLD) {
            GPIO_Write(LED_PIN, GPIO_LOW);
        } else {
            GPIO_Write(LED_PIN, GPIO_HIGH);
        }

        Delay(100);
    }
}