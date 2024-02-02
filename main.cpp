//#include <stdio.h>
#include <device/usbd.h>
#include "rgb/rgb.h"
#include "rgb/RGBController.h"
#include "rotary_encoder/RotaryEncoder.h"
#include "LCDLibrary.h"
#include "keys/KeyManager.h"
#include "tusb.h"
#include "usb/main.h"
#include "keys/KeyLayerManager.h"

uint8_t numKeys = 14;
uint8_t keyPins[] = {
        2, /* display */ 27,
        4, 17, 20, 23,
        5, 18, 21, 24,
        6, 19, 22, 25
};

KeyManager keys(keyPins, numKeys);

KeyLayerManager layers(&keys);

RotaryEncoder enc0(0, 1);
RotaryEncoder enc1(29, 28);

PIOSPIInterface lcd_spi(pio1, 10, 9, 11, 12);
LCDDirectGraphics lcd(&lcd_spi, 13, 14, 52, 40, 135, 240, Degree_90);

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
    keys.setupPins();
    initEncoder();

    rgb_init(pio0, 3);
    RGBController rgb(12);

    rgb.setAll(0);
    rgb.write();

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0x0000);

    std::unique_ptr<KeyAction[]> numPadActions(new KeyAction[numKeys]);
    numPadActions[10] = {KEY_PRESS, HID_KEY_KEYPAD_0};
    numPadActions[3] = {KEY_PRESS, HID_KEY_KEYPAD_7};
    numPadActions[4] = {KEY_PRESS, HID_KEY_KEYPAD_8};
    numPadActions[5] = {KEY_PRESS, HID_KEY_KEYPAD_9};
    numPadActions[7] = {KEY_PRESS, HID_KEY_KEYPAD_4};
    numPadActions[8] = {KEY_PRESS, HID_KEY_KEYPAD_5};
    numPadActions[9] = {KEY_PRESS, HID_KEY_KEYPAD_6};
    numPadActions[11] = {KEY_PRESS, HID_KEY_KEYPAD_1};
    numPadActions[12] = {KEY_PRESS, HID_KEY_KEYPAD_2};
    numPadActions[13] = {KEY_PRESS, HID_KEY_KEYPAD_3};
    uint8_t numPadLayer = layers.addLayer(std::move(numPadActions));

    std::unique_ptr<KeyAction[]> rootActions(new KeyAction[numKeys]);
    rootActions[2] = {ENABLE_OVERLAY_LAYER, numPadLayer};
    rootActions[8] = {KEY_PRESS, HID_KEY_ARROW_UP};
    rootActions[11] = {KEY_PRESS, HID_KEY_ARROW_LEFT};
    rootActions[12] = {KEY_PRESS, HID_KEY_ARROW_DOWN};
    rootActions[13] = {KEY_PRESS, HID_KEY_ARROW_RIGHT};
    uint8_t rootLayer = layers.addLayer(std::move(rootActions));

    layers.setRootLayer(rootLayer);

    tusb_init();

    while (true) {
        tud_task();
        keys.tick();
        layers.tick();
        for (int i = 0; i < 12; ++i) {
            int32_t value = 0;
            if (keys.isActive(i + 2)) {
                value = 255;
            } else if (keys.timeInactive(i + 2) > 0) {
                value = 255 - (keys.timeInactive(i + 2) / 4);
                if (value < 0) value = 0;
                if (value > 255) value = 255;
            }
            rgb.setPixel(i, value << 8);
        }
        rgb.write();
        sleep_ms(1);
        hid_task(&layers);
    }

    return 0;
}
