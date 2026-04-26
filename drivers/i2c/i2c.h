#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <avr/io.h>  

void I2C_Init(void);
uint8_t I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Write(uint8_t data);

#endif // I2C_H