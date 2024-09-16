
#include "KeyListener.h"

KeyListener::KeyListener(KeyStateManager *manager, std::unique_ptr<HWKeyConfig[]> keys, uint8_t numKeys)
        : manager(manager), keys(std::move(keys)), numKeys(numKeys) {}

void KeyListener::setupPins() {
    for (int i = 0; i < numKeys; ++i) {
        uint pin = keys[i].pin;
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
    }
}

void KeyListener::tick(absolute_time_t timestamp) {
    for (int i = 0; i < numKeys; ++i) {
        HWKeyConfig &key = keys[i];
        bool isPressed = !gpio_get(key.pin);
        manager->updateKeyState(key.id, isPressed, timestamp);
    }
}
