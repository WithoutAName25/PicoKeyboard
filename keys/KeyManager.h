#ifndef PICOKEYBOARD_KEYMANAGER_H
#define PICOKEYBOARD_KEYMANAGER_H

#include <memory>
#include "pico/stdio.h"

struct KeyState {
    uint8_t keyPin;
    bool isActive;
    uint32_t activeSinceTick;
    uint32_t inactiveSinceTick;
};

class KeyManager {
private:
    uint32_t currentTick;
    uint8_t numKeys;
    std::unique_ptr<KeyState[]> keyStates;

public:
    explicit KeyManager(uint8_t *keyPins, uint8_t numKeys);

    void setupPins();

    void tick();

    [[nodiscard]] bool isActive(uint8_t key) const;

    [[nodiscard]] uint32_t timeActive(uint8_t key) const;

    [[nodiscard]] uint32_t timeInactive(uint8_t key) const;

    [[nodiscard]] uint8_t getNumKeys() const;
};


#endif //PICOKEYBOARD_KEYMANAGER_H
