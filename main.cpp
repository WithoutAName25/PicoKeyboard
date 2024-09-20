#include "main.h"
#include "rotary_encoder/RotaryEncoder.h"
#include "lib/LCDLibrary/spi/PioSPIInterface.h"
#include "lib/LCDLibrary/graphics/LCDDirectGraphics.h"
#include "games/pong/PongGame.h"
#include "rgb/rgb.h"
#include "rgb/RGBController.h"
#include "rgb/effect/RainbowEffect.h"

uint8_t numKeys = 14;
uint8_t keyPins[] = {
        2, /* display */ 27,
        4, 17, 20, 23,
        5, 18, 21, 24,
        6, 19, 22, 25
};

RotaryEncoder enc0(0, 1);
RotaryEncoder enc1(29, 28);

PIOSPIInterface lcd_spi(pio1, 10, 9, 11, 12);
LCDDirectGraphics lcd(&lcd_spi, 13, 14, 52, 40, 135, 240, Degree_90);

PongGame pong(&enc0, &enc1, &lcd);

RGBController rgb(24);

Scheduler scheduler;

KeyStateController keyStateController(numKeys);

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

    rgb.setAll(0);
    rgb.write();

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0);

//    tusb_init();
//
//
//    while (true) {
//        tud_task();
//        hid_task(&layers);
//    }

    rgb.setEffect(std::make_unique<RainbowEffect>());
    scheduler.run();

    return 0;
}
