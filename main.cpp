#include <stdio.h>
#include "rgb/rgb.h"
#include "rgb/RGBController.h"
#include "rotary_encoder/RotaryEncoder.h"
#include "LCDLibrary.h"
#include "keys/KeyManager.h"

uint8_t numKeys = 12;
uint8_t keyPins[] = {
        4, 17, 20, 23,
        5, 18, 21, 24,
        6, 19, 22, 25,
        2, 27
};

KeyManager keys(keyPins, numKeys);

RotaryEncoder enc0(0, 1);
RotaryEncoder enc1(29, 28);

SPIInterface *lcd_spi = new PIOSPIInterface(pio1, 10, 9, 11, 12);
LCDDirectGraphics lcd(lcd_spi, 13, 14, 52, 40, 135, 240);

void interrupt_callback(uint gpio, uint32_t events) {
    enc0.callback(gpio);
    enc1.callback(gpio);
}

void initEncoder() {
    irq_set_enabled(IO_IRQ_BANK0, true);
    gpio_set_irq_callback(interrupt_callback);
    enc0.init();
    enc1.init();
}

int main() {
    stdio_init_all();
    keys.setupPins();
    initEncoder();

    rgb_init(pio0, 3);
    RGBController rgb(12);

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0x0000);

    while (true) {
        keys.tick();
        for (int i = 0; i < 12; ++i) {
            int32_t value = 0;
            if (keys.isActive(i)) {
                value = 255;
            } else if (keys.timeInactive(i) > 0) {
                value = 255 - (keys.timeInactive(i) / 4);
                if (value < 0) value = 0;
                if (value > 255) value = 255;
            }
            rgb.setPixel(i, value << 8);
        }
        rgb.write();
        sleep_ms(1);
    }

    return 0;
}
