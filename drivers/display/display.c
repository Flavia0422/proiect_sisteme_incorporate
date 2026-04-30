#include "drivers/display/display.h"
#include "drivers/i2c/i2c.h"
#include <string.h>

//din datasheet:
// SSD1306 I2C address
#define SSD1306_ADDR        0x78  // 0x3C << 1 shift pentru a face loc bitului de R/W

// Control bytes
#define SSD1306_CMD         0x00 // Colectie de comenzi
#define SSD1306_DATA        0x40 // valoarea reprezinta faptul ca urmatorii bytes trimisi dupa acest control byte vor fi 
// interpretati ca date pentru display, nu comenzi

// Display dimensions
#define DISPLAY_WIDTH       128 // pixeli
#define DISPLAY_PAGES       8    // 8 pagini x 8 pixeli = 64 pixeli inaltime

// Frame buffer
static uint8_t buffer[DISPLAY_PAGES][DISPLAY_WIDTH];

// fontul in cod ASCII 5x7, fiecare caracter este reprezentat de 5 bytes
// (5 coloane a cate 8 biti, dar doar 7 sunt folositi pentru inaltime)
static const uint8_t font5x7[][5] = {
    {0x7E,0x11,0x11,0x11,0x7E}, // 'A'
    {0x7F,0x49,0x49,0x49,0x36}, // 'B'
    {0x3E,0x41,0x41,0x41,0x22}, // 'C'
    {0x7F,0x41,0x41,0x22,0x1C}, // 'D'
    {0x7F,0x49,0x49,0x49,0x41}, // 'E'
    {0x7F,0x09,0x09,0x09,0x01}, // 'F'
    {0x3E,0x41,0x49,0x49,0x7A}, // 'G'
    {0x7F,0x08,0x08,0x08,0x7F}, // 'H'
    {0x00,0x41,0x7F,0x41,0x00}, // 'I'
    {0x20,0x40,0x41,0x3F,0x01}, // 'J'
    {0x7F,0x08,0x14,0x22,0x41}, // 'K'
    {0x7F,0x40,0x40,0x40,0x40}, // 'L'
    {0x7F,0x02,0x0C,0x02,0x7F}, // 'M'
    {0x7F,0x04,0x08,0x10,0x7F}, // 'N'
    {0x3E,0x41,0x41,0x41,0x3E}, // 'O'
    {0x7F,0x09,0x09,0x09,0x06}, // 'P'
    {0x3E,0x41,0x51,0x21,0x5E}, // 'Q'
    {0x7F,0x09,0x19,0x29,0x46}, // 'R'
    {0x46,0x49,0x49,0x49,0x31}, // 'S'
    {0x01,0x01,0x7F,0x01,0x01}, // 'T'
    {0x3F,0x40,0x40,0x40,0x3F}, // 'U'
    {0x1F,0x20,0x40,0x20,0x1F}, // 'V'
    {0x3F,0x40,0x38,0x40,0x3F}, // 'W'
    {0x63,0x14,0x08,0x14,0x63}, // 'X'
    {0x07,0x08,0x70,0x08,0x07}, // 'Y'
    {0x61,0x51,0x49,0x45,0x43}, // 'Z'
};

static void ssd1306_cmd(uint8_t cmd) { // functie pentru a trimite o comanda catre display folosind protocolul I2C
    // apelare a functiilor i2c pentru a transmite comanda
    I2C_Start(SSD1306_ADDR);
    I2C_Write(SSD1306_CMD);
    I2C_Write(cmd);
    I2C_Stop();
}

static void ssd1306_data(uint8_t data) { 
    // functie pentru a trimite un byte de date catre display
    I2C_Start(SSD1306_ADDR);
    I2C_Write(SSD1306_DATA);
    I2C_Write(data);
    I2C_Stop();
}

void display_init(void)
{
   I2C_Init(); // initializare modul I2C
    // secventa de initializare conform datasheet-ului SSD1306
    ssd1306_cmd(0xAE); // pentru display OFF
    ssd1306_cmd(0xD5); // setare frecventa de refresh
    ssd1306_cmd(0x80); // valoarea pentru clock divide
    ssd1306_cmd(0xA8); // setare inaltime display
    ssd1306_cmd(0x3F); // valoarea pentru 64 pixeli


}

void Display_Clear(void) { // functie pentru a goli bufferul de date (stergere ecran)

  memset(buffer, 0, sizeof(buffer));

}