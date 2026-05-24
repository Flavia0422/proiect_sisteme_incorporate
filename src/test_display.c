#include <avr/io.h>
#include "drivers/display/display.h"
#include "utils/delay.h"

int main(void) {
    Display_Init();

    // Testare de baza: afisare text, paginare, actualizare display
    Display_Clear();
    Display_Print(0, 0, "SAFE BOX");
    Display_Print(0, 2, "Hello World!");
    Display_Print(0, 4, "Testing 1234");
    Display_Update();

    Delay(3000);

    // Testare paginare: afisare text diferit pe fiecare pagina
    Display_Clear();
    Display_Print(0, 0, "Page 0");
    Display_Print(0, 1, "Page 1");
    Display_Print(0, 2, "Page 2");
    Display_Print(0, 3, "Page 3");
    Display_Print(0, 4, "Page 4");
    Display_Print(0, 5, "Page 5");
    Display_Print(0, 6, "Page 6");
    Display_Print(0, 7, "Page 7");
    Display_Update();

    Delay(3000);

    // Testare blink: afisare text la intervale regulate
    while (1) {
        Display_Clear();
        Display_Print(0, 3, "  DISPLAY OK!");
        Display_Update();
        Delay(500);

        Display_Clear();
        Display_Update();
        Delay(500);
    }
}