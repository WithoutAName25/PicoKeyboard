#include "../KeyMatrixListener.h"

#include <ranges>

KeyMatrixListener::KeyMatrixListener(KeyStateController &controller, const uint8_t numKeys, HWMatrixKeyConfig *keys)
    : IKeyListener(controller, numKeys) {
    for (uint8_t i = 0; i < numKeys; i++) {
        HWMatrixKeyConfig *key = &keys[i];
        if (!keysPerInPin.contains(key->pinIn)) {
            keysPerInPin.emplace(key->pinIn, std::vector<HWMatrixKeyConfig *>());
        }

        keysPerInPin[key->pinIn].push_back(key);
        outPins.emplace(key->pinOut);
    }
}

void KeyMatrixListener::setupPins() {
    for (const uint8_t &inPin : keysPerInPin | std::views::keys) {
        gpio_init(inPin);
        gpio_set_dir(inPin, GPIO_OUT);
        gpio_put(inPin, false);
    }
    for (const uint8_t &outPin : outPins) {
        gpio_init(outPin);
        gpio_set_dir(outPin, GPIO_IN);
        gpio_pull_down(outPin);
    }
}

void KeyMatrixListener::execute(const absolute_time_t timestamp) {
    for (const auto &[inPin, keys] : keysPerInPin) {
        gpio_put(inPin, true);
        sleep_us(5);
        for (const auto key : keys) {
            const bool isPressed = gpio_get(key->pinOut);
            controller.updateKeyState(key->id, isPressed, timestamp, true);
        }
        gpio_put(inPin, false);
    }
}
