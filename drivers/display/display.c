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
    ssd1306_cmd(0xD3);//setare offset vertical
    ssd1306_cmd(0x00);//offset 0 -fara deplasare
    ssd1306_cmd(0x40); //setare start linie
    ssd1306_cmd(0x8D);//setare charge pump-adica modul de alimentare
    ssd1306_cmd(0x14);//activare a charge pump-ului pentru a putea alimenta display-ul cu tensiunea necesara
    ssd1306_cmd(0x20); //setare mod de adresare orizontala
    ssd1306_cmd(0x00);//tot pentru mod orizontal - nu folosim paginare sau adresare verticala
    ssd1306_cmd(0xA1);//segment remap -pentru a inversa ordinea coloanelor (col 127 devine col 0) , deoarece in modul orizontal adresa incepe de la col 0
    //in modul orizontal, adresa incepe de la col 0, dar pentru a avea o orientare corecta a display-ului, segment remap este necesar pentru a face ca col 127 sa devina col 0
    //adica se face o oglindire pe verticala a coloanelor, astfel incat textul sa fie afisat corect
    ssd1306_cmd(0xC8);//COM scan direction -pentru a inversa ordinea paginilor (pagina 7 devine pagina 0) , deoarece in modul orizontal adresa incepe de la pagina 0
    ssd1306_cmd(0xDA);//COM pins- pentru a seta configuratia pinilor COM (conexiunile dintre driverul de display si matricea de pixeli)
    ssd1306_cmd(0x12);//valoarea pentru configuratia COM -cu 64 pixeli inaltime
    ssd1306_cmd(0x81); //setare contrast
    ssd1306_cmd(0xCF); //valoarea pentru contrast -ajustare pentru a avea o luminozitate buna
    ssd1306_cmd(0xD9); //setare pre-charge period
    ssd1306_cmd(0xF1); //valoarea pentru pre-charge-ajustare pentru a avea echilibru intre luminozitate si consum
    ssd1306_cmd(0xDB); //setare VCOMH deselect level-adica nivelul de tensiune pentru semnalul VCOMH, care afecteaza contrastul si stabilitatea imaginii
    ssd1306_cmd(0x40); //valoarea pentru VCOMH - pentru a avea o imagine stabila 
    ssd1306_cmd(0xA4); //se seteaza pentru a afisa continutul bufferului (daca nu era setat, display-ul ar ignora datele trimise si ar ramane negru)
    ssd1306_cmd(0xA6);//setare pentru a afisa pixeli normali (daca nu era setat, display-ul ar afisa pixeli inversati, adica 0 ar fi alb si 1 ar fi negru)
    ssd1306_cmd(0xAF); //pentru display ON
    Display_Clear(); // golire buffer pentru a incepe cu un ecran curat
Display_Update();//actualizare display pentru a reflecta bufferul gol (afisare ecran curat)


}

void Display_Clear(void) { // functie pentru a goli bufferul de date (stergere ecran)

  memset(buffer, 0, sizeof(buffer));

}

void Display_Update(void) 
{
for(uint8_t page = 0; page<DISPLAY_PAGES;page++)
{
    ssd1306_cmd(0xB0+page);//setare a paginii curente
    ssd1306_cmd(0x10);//setare a adresei pt coloana(partea superioara) -primii 4 biti
    ssd1306_cmd(0x00);//setare a adresei pt coloana(partea de jos)-ultimii 4 biti


//.....


}

}