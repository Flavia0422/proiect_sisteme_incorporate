#include "i2c.h"
#include <avr/io.h>

//folosind formula din datasheet pentru calculul 
//valorii de TWBR in functie de frecventa 
// F_CPU = 16MHz, target = 400kHz
// TWBR = (F_CPU / target - 16) / (2 * prescaler)
// TWBR = (16000000 / 400000 - 16) / 2 = 12
#define I2C_TWBR    12 //pentru clock speed de 400kHz
#define I2C_TWSR    0x00  // prescaler = 1

static void i2c_wait(void) {
    while (!(TWCR & (1 << TWINT)));
    /* din datasheet:
    Wait for TWINT flag set. This
    indicates that the START
    condition has been transmitted
    */
}

void I2C_Init(void) {
    // cei 2 registrii de configurare a vitezei
    TWSR = I2C_TWSR;
    TWBR = I2C_TWBR;
    //enable pentru TWI
    TWCR = (1 << TWEN);



}

uint8_t I2C_Start(void) {
    // Set TWINT to clear flag,
    // TWSTA to request START, 
    //TWEN to keep enabled
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    i2c_wait();

 //.....

    
}

void I2C_Stop(void) {}

uint8_t I2C_Write(uint8_t data) {}