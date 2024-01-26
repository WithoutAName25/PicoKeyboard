#include "KeyManager.h"
#include "pico/stdlib.h"

KeyManager::KeyManager(uint8_t *keyPins, uint8_t numKeys) : currentTick(0), numKeys(numKeys), keyStates(new KeyState[numKeys]) {
    for (int i = 0; i < numKeys; ++i) {
        keyStates[i] = {
                .keyPin = keyPins[i],
                .isActive = false,
                .activeSinceTick = static_cast<uint32_t>(-1), // max value
                .inactiveSinceTick = static_cast<uint32_t>(-1) // max value
        };
    }
}

KeyManager::~KeyManager() {
    delete[] keyStates;
}

void KeyManager::setupPins() {
    for (int i = 0; i < numKeys; ++i) {
        uint pin = keyStates[i].keyPin;
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
    }
}

void KeyManager::tick() {
    currentTick++;
    for (int i = 0; i < numKeys; ++i) {
        bool newState = !gpio_get(keyStates[i].keyPin);
        if (keyStates[i].isActive != newState) {
            keyStates[i].isActive = newState;
            if (newState) {
                keyStates[i].activeSinceTick = currentTick;
            } else {
                keyStates[i].inactiveSinceTick = currentTick;
            }
        }
    }
}

bool KeyManager::isActive(uint8_t key) const {
    return keyStates[key].isActive;
}

uint32_t KeyManager::timeActive(uint8_t key) const {
    if (currentTick < keyStates[key].activeSinceTick) return -1;
    return currentTick - keyStates[key].activeSinceTick;
}

uint32_t KeyManager::timeInactive(uint8_t key) const {
    if (currentTick < keyStates[key].inactiveSinceTick) return -1;
    return currentTick - keyStates[key].inactiveSinceTick;
}
