#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void Display_Init(void);
void Display_Clear(void);
void Display_Print(uint8_t x, uint8_t page, const char *str);
void Display_Char(uint8_t x, uint8_t page, char c);
void Display_Update(void);

#endif // DISPLAY_H