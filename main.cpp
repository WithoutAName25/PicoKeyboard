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
#include "games/pong/PongGame.h"

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

PongGame pong(&enc0, &enc1, &lcd);

Mode lastMode = MODE_DEFAULT;

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
    keys.setupPins();
    initEncoder();

    rgb_init(pio0, 3);
    RGBController rgb(12);

    rgb.setAll(0);
    rgb.write();

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0);

    configureKeys();

    tusb_init();

    uint64_t time = time_us_64();
    uint64_t lastTime = time;
    uint64_t dTime = 0;
    uint64_t lastPongUpdate = 0;

    uint8_t timesSkipped = 0;

    while (true) {
        tud_task();
        keys.tick();
        layers.tick();
        hid_task(&layers);
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

//            // Show times skipped
//            if (timesSkipped > 12) timesSkipped = 1;
//            if (timesSkipped > i) {
//                rgb.setPixel(i, 255 << 16);
//            } else {
//                rgb.setPixel(i, 0);
//            }
        }
        rgb.write();
        Mode mode = layers.getMode();
        if (lastMode != mode) {
            switch (lastMode) {
                case MODE_PONG:
                    lcd.setBrightness(0);
                    lcd.clear(0);
                    break;
                default:
                    break;
            }
            switch (mode) {
                case MODE_PONG:
                    lcd.setBrightness(255);
                    pong.init();
                    break;
                default:
                    break;
            }
            lastMode = mode;
        }
        switch (mode) {
            case MODE_PONG:
                if (lastPongUpdate + 20 * 1000 < time) {
                    lastPongUpdate = time;
                    pong.update();
                    pong.draw();
                }
                break;
            default:
                break;
        }


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

void configureKeys() {
    auto [layerSwitcherID, layerSwitcherActions] = layers.addLayer();
    auto [mainRootID, mainRootActions] = layers.addLayer();
    auto [numPadID, numPadActions] = layers.addLayer();
    auto [modeSelectorID, modeSelectorActions] = layers.addLayer();

    layerSwitcherActions[2] = {ROOT_LAYER_SWITCH, mainRootID};
    layerSwitcherActions[3] = {ROOT_LAYER_SWITCH, modeSelectorID};
    layerSwitcherActions[4] = {ROOT_LAYER_SWITCH, mainRootID};
    layerSwitcherActions[5] = {ROOT_LAYER_SWITCH, mainRootID};
    layerSwitcherActions[6] = {ROOT_LAYER_SWITCH, mainRootID};
    layerSwitcherActions[7] = {ROOT_LAYER_SWITCH, mainRootID};
    layerSwitcherActions[8] = {ROOT_LAYER_SWITCH, mainRootID};
    layerSwitcherActions[9] = {ROOT_LAYER_SWITCH, mainRootID};

    mainRootActions[2] = {ENABLE_OVERLAY_LAYER, numPadID};
    mainRootActions[10] = {ENABLE_OVERLAY_LAYER, layerSwitcherID};

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

    modeSelectorActions[10] = {ENABLE_OVERLAY_LAYER, layerSwitcherID};
    modeSelectorActions[2] = {SWITCH_MODE, MODE_DEFAULT};
    modeSelectorActions[3] = {SWITCH_MODE, MODE_PONG};

    layers.setRootLayer(mainRootID);
}
