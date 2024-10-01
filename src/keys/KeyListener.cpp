
#include "KeyListener.h"

KeyListener::KeyListener(KeyStateController &controller, HWKeyConfig *keys, uint8_t numKeys)
        : controller(controller), keys(keys), numKeys(numKeys) {
}

void KeyListener::setupPins() {
    for (int i = 0; i < numKeys; ++i) {
        uint pin = keys[i].pin;
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
    }
}

void KeyListener::execute(absolute_time_t timestamp) {
    for (int i = 0; i < numKeys; ++i) {
        HWKeyConfig &key = keys[i];
        bool isPressed = !gpio_get(key.pin);
        controller.updateKeyState(key.id, isPressed, timestamp);
    }
}
