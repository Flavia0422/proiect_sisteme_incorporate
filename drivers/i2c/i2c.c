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

uint8_t status = TWSR & 0xF8;//masca pentru a extrage doar primii 5 biti (statusul)
// 0xF8 = 1111 1000, astfel eliminam ultimii 3 biti care pot 
//contine informatii despre adresa sau date, lasand doar codul de status relevant pentru verificare
    return (status == 0x08 || status == 0x10) ? 1 : 0;
    //din datasheet:
    //0x08 = START transmitted
    //0x10 = Repeated START transmitted
//succes daca START sau repeated START a fost transmis
    
}

void I2C_Stop(void) {
    //din datasheet: transmit STOP condition
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); 
    // linia asta seteaza bitii necesari pentru a transmite o conditie de STOP pe bus-ul I2C
    // TWINT = 1 pentru a clear flag-ul si a permite continuarea operatiunii
    // TWSTO = 1 pentru a indica ca vrem sa transmitem o conditie de STOP
    // TWEN = 1 pentru a asigura ca TWI este in continuare activat
    
}

uint8_t I2C_Write(uint8_t data) {
    //din datasheet: load data into TWDR register. Clear TWINT to start transmission of data
    TWDR = data;
    TWCR = (1<< TWINT) | (1<< TWEN);
    i2c_wait();
    uint8_t status = TWSR & 0xF8; //masca pentru a extrage doar primii 5 biti (statusul)
    return (status == 0x18 || status == 0x28) ? 1 : 0;
    

}