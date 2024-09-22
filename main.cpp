#include "main.h"
#include "rotary_encoder/RotaryEncoder.h"
#include "lib/LCDLibrary/spi/PioSPIInterface.h"
#include "lib/LCDLibrary/graphics/LCDDirectGraphics.h"
#include "games/pong/PongGame.h"
#include "rgb/rgb.h"
#include "rgb/effect/RainbowWaveEffect.h"
#include "keys/KeyListener.h"
#include "rgb/effect/KeyStateEffect.h"

uint8_t numKeys = 14;
HWKeyConfig keys[] = {
        {2,  0},
        {27, 1},
        {4,  2},
        {17, 3},
        {20, 4},
        {23, 5},
        {5,  6},
        {18, 7},
        {21, 8},
        {24, 9},
        {6,  10},
        {19, 11},
        {22, 12},
        {25, 13}
};

uint8_t numLEDs = 23;
LedConfig ledConfigs[] = {
        LedConfig(0, 0, 64, 88.8, 2),
        LedConfig(1, 1, 82, 88.8, 3),
        LedConfig(2, 2, 100, 88.8, 4),
        LedConfig(3, 3, 118, 88.8, 5),
        LedConfig(4, 4, 64, 105.8, 6),
        LedConfig(5, 5, 82, 105.8, 7),
        LedConfig(6, 6, 100, 105.8, 8),
        LedConfig(7, 7, 118, 105.8, 9),
        LedConfig(8, 8, 64, 122.8, 10),
        LedConfig(9, 9, 82, 122.8, 11),
        LedConfig(10, 10, 100, 122.8, 12),
        LedConfig(11, 11, 118, 122.8, 13),
        LedConfig(12, 12, 122, 52.5),
        LedConfig(13, 13, 129.5, 60),
        LedConfig(14, 14, 129.5, 95.5),
        LedConfig(15, 15, 129.5, 131),
        LedConfig(16, 16, 121.5, 138.5),
        LedConfig(17, 17, 91, 138.5),
        LedConfig(18, 18, 60, 138.5),
        LedConfig(19, 19, 52.5, 131),
        LedConfig(20, 20, 52.5, 95.5),
        LedConfig(21, 21, 52.5, 60),
        LedConfig(22, 22, 60, 52.5),
};

RotaryEncoder enc0(0, 1);
RotaryEncoder enc1(29, 28);

PIOSPIInterface lcd_spi(pio1, 10, 9, 11, 12);
LCDDirectGraphics lcd(&lcd_spi, 13, 14, 52, 40, 135, 240, Degree_90);

PongGame pong(&enc0, &enc1, &lcd);

Scheduler scheduler;

RgbController rgb(scheduler, ledConfigs, numLEDs);

KeyStateController keyStateController(numKeys);

KeyListener keyListener(scheduler, keyStateController, keys, numKeys);

KeyActionController keyActionController(numKeys, keyStateController);

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

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0);

    keyListener.setupPins();

//    tusb_init();
//
//
//    while (true) {
//        tud_task();
//        hid_task(&layers);
//    }

    rgb.setEffect(std::make_unique<KeyStateEffect>(std::make_unique<RainbowWaveEffect>(), 3000));
    scheduler.run();

    return 0;
}
