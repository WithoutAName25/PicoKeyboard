//#include <stdio.h>
#include <device/usbd.h>
#include "rgb/rgb.h"
#include "rgb/RGBController.h"
#include "rotary_encoder/RotaryEncoder.h"
#include "LCDLibrary.h"
#include "keys/KeyStateManager.h"
#include "tusb.h"
#include "usb/main.h"
#include "games/pong/PongGame.h"

uint8_t numKeys = 14;
uint8_t keyPins[] = {
        2, /* display */ 27,
        4, 17, 20, 23,
        5, 18, 21, 24,
        6, 19, 22, 25
};

KeyStateManager keys(numKeys);

RotaryEncoder enc0(0, 1);
RotaryEncoder enc1(29, 28);

PIOSPIInterface lcd_spi(pio1, 10, 9, 11, 12);
LCDDirectGraphics lcd(&lcd_spi, 13, 14, 52, 40, 135, 240, Degree_90);

PongGame pong(&enc0, &enc1, &lcd);

void configureKeys();

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
//    stdio_init_all();
    initEncoder();

    rgb_init(pio0, 3);
    RGBController rgb(12);

    rgb.setAll(255 << 8);
    rgb.write();

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0);

    tusb_init();

    uint64_t time = time_us_64();
    uint64_t lastTime = time;
    uint64_t dTime = 0;
    uint64_t lastPongUpdate = 0;

    uint8_t timesSkipped = 0;

    while (true) {
        tud_task();
//        hid_task(&layers);


        uint64_t timeUsed = time_us_64() - time;
        if (timeUsed < 1000) {
            sleep_us(1000 - timeUsed);
        } else {
            timesSkipped += 1;
        }
        lastTime = time;
        time = time_us_64();
        dTime = time - lastTime;
    }

    return 0;
}
