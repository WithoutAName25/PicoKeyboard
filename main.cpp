#include "LCDLibrary.h"

LCDDirectGraphics lcd(
        spi1,
        11,
        10,
        12,
        9,
        8,
        7,
        52,
        40,
        135,
        240
);

int main() {
    lcd.init();
    lcd.clear(0x0000);
    lcd.fillRect(50, 10, 84, 20, 0xFFFF);
    lcd.fillRect(50, 220, 84, 230, 0xFFFF);
    return 0;
}
