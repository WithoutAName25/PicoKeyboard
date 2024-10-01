#include "LCDLibrary.h"

#include "keys.h"
#include "rgb.h"
#include "rgb_effects.h"
#include "rotary_encoder.h"
#include "usb.h"

//// ------------------------------------
//// Makropad configuration
//// ------------------------------------
//
//uint8_t numKeys = 14;
//HWKeyConfig keys[] = {
//        {2,  0},
//        {27, 1},
//        {4,  2},
//        {17, 3},
//        {20, 4},
//        {23, 5},
//        {5,  6},
//        {18, 7},
//        {21, 8},
//        {24, 9},
//        {6,  10},
//        {19, 11},
//        {22, 12},
//        {25, 13}
//};
//
//uint8_t ledPin = 3;
//uint8_t numLEDs = 23;
//LedConfig ledConfigs[] = {
//        LedConfig(0, 0, 64, 88.8, 2),
//        LedConfig(1, 1, 82, 88.8, 3),
//        LedConfig(2, 2, 100, 88.8, 4),
//        LedConfig(3, 3, 118, 88.8, 5),
//        LedConfig(4, 4, 64, 105.8, 6),
//        LedConfig(5, 5, 82, 105.8, 7),
//        LedConfig(6, 6, 100, 105.8, 8),
//        LedConfig(7, 7, 118, 105.8, 9),
//        LedConfig(8, 8, 64, 122.8, 10),
//        LedConfig(9, 9, 82, 122.8, 11),
//        LedConfig(10, 10, 100, 122.8, 12),
//        LedConfig(11, 11, 118, 122.8, 13),
//        LedConfig(12, 12, 122, 52.5),
//        LedConfig(13, 13, 129.5, 60),
//        LedConfig(14, 14, 129.5, 95.5),
//        LedConfig(15, 15, 129.5, 131),
//        LedConfig(16, 16, 121.5, 138.5),
//        LedConfig(17, 17, 91, 138.5),
//        LedConfig(18, 18, 60, 138.5),
//        LedConfig(19, 19, 52.5, 131),
//        LedConfig(20, 20, 52.5, 95.5),
//        LedConfig(21, 21, 52.5, 60),
//        LedConfig(22, 22, 60, 52.5),
//};
//
//RotaryEncoder enc0(0, 1);
//RotaryEncoder enc1(29, 28);
//
//PIOSPIInterface lcd_spi(pio1, 10, 9, 11, 12);
//LCDDirectGraphics lcd(&lcd_spi, 13, 14, 52, 40, 135, 240, Degree_90);
//
//PongGame pong(&enc0, &enc1, &lcd);

// ------------------------------------
// KeySplit left configuration
// ------------------------------------

uint8_t numKeys = 18;
HWKeyConfig keys[] = {
        {5,  0}, // short-circuit
        {4,  1},
        {3,  2},
        {2,  3},
        {1,  4},
        {15, 5},
        {14, 6},
        {13, 7},
        {7,  8},
        {6,  9},
        {21, 10},
        {22, 11},
        {26, 12},
        {27, 13},
        {28, 14}, // short-circuit
        {20, 15}, // short-circuit
        {19, 16}, // short-circuit
        {18, 17} // short-circuit
};

uint8_t ledPin = 0;
uint8_t numLEDs = 18;
LedConfig ledConfigs[] = {
        LedConfig(0, 0, 0, -35, 0),
        LedConfig(1, 1, 18.5, -43.75, 1),
        LedConfig(2, 2, 37, -48.125, 2),
        LedConfig(3, 3, 55.5, -43.75, 3),
        LedConfig(4, 4, 74, -41.5625, 4),
        LedConfig(5, 5, 0, -17.5, 5),
        LedConfig(6, 6, 18.5, -26.25, 6),
        LedConfig(7, 7, 37, -30.625, 7),
        LedConfig(8, 8, 55.5, -26.25, 8),
        LedConfig(9, 9, 74, -24.0625, 9),
        LedConfig(10, 10, 0, 0, 10),
        LedConfig(11, 11, 18.5, -8.75, 11),
        LedConfig(12, 12, 37, -13.125, 12),
        LedConfig(13, 13, 55.5, -8.75, 13),
        LedConfig(14, 14, 74, -6.5625, 14),
        LedConfig(15, 15, 55.5, 13.125, 15),
        LedConfig(16, 16, 77.3667826, 15.0380956, 16),
        LedConfig(17, 17, 98.5691541, 20.7192539, 17)
};

PIOSPIInterface lcd_spi(pio1, 11, 10, 9, 8);
LCDDirectGraphics lcd(&lcd_spi, 12, 25, 52, 40, 135, 240, Degree_90);


Scheduler scheduler;

RGBController rgb(ledConfigs, numLEDs);

KeyStateController keyStateController(numKeys);

KeyListener keyListener(keyStateController, keys, numKeys);

KeyActionController keyActionController(numKeys, keyStateController);

HIDKeyboard hidKeyboard;
HIDMouse hidMouse;
HIDConsumerControl hidConsumerControl;
HIDGamepad hidGamepad;
HIDCommunication hidCommunication;

TUDTask tudTask;

void configureKeys();

//void interrupt_callback(uint gpio, uint32_t events) {
//    enc0.callback(gpio);
//    enc1.callback(gpio);
//}

void initEncoder() {
//    irq_set_enabled(IO_IRQ_BANK0, true);
//    gpio_set_irq_callback(interrupt_callback);
//    enc0.init();
//    enc1.init();
}

int main() {
//    stdio_init_all();
    initEncoder();

    rgb_init(pio0, ledPin);

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0);

    keyListener.setupPins();

    tud_init(BOARD_TUD_RHPORT);

    scheduler.addPeriodicTask(&rgb, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&keyListener, get_absolute_time(), 500);

    scheduler.addPeriodicTask(&tudTask, get_absolute_time(), 500);

    scheduler.addPeriodicTask(&hidKeyboard, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&hidMouse, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&hidConsumerControl, get_absolute_time(), 10000);
    scheduler.addPeriodicTask(&hidGamepad, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&hidCommunication, get_absolute_time(), 10000);

//    rgb.setEffect(std::make_unique<KeyStateEffect>(std::make_unique<RainbowWaveEffect>(), 3000));
    rgb.setEffect(std::make_unique<RainbowWaveEffect>());
    scheduler.run();

    return 0;
}
