#include <stdio.h>
#include "rgb/rgb.h"
#include "rgb/RGBController.h"
#include "rotary_encoder/RotaryEncoder.h"
#include "LCDLibrary.h"

uint8_t numKeys = 12;
uint8_t keyPins[] = {
        4, 17, 20, 23,
        5, 18, 21, 24,
        6, 19, 22, 25,
        2, 27
};

RotaryEncoder enc0(0, 1);
RotaryEncoder enc1(29, 28);

void initKeys() {
    for (uint8_t keyPin: keyPins) {
        gpio_init(keyPin);
        gpio_set_dir(keyPin, GPIO_IN);
        gpio_pull_up(keyPin);
    }
}

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
    initKeys();
    initEncoder();

    rgb_init(pio0, 3);
    RGBController rgb(12);

    SPIInterface *lcd_spi = new PIOSPIInterface(pio1, 10, 9, 11, 12);
    LCDDirectGraphics lcd(lcd_spi, 13, 14, 52, 40, 135, 240);

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0x0000);

    uint lastColor = 0;
    while (true) {
        for (int i = 0; i < 12; ++i) {
            bool state = gpio_get(keyPins[i]);
            rgb.setPixel(i, state ? 0 : colorRGB(0, 0, 255));
        }
        rgb.write();
        sleep_ms(1);
    }

    return 0;
}
