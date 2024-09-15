#ifndef PICOKEYBOARD_KEYSTATEMANAGER_H
#define PICOKEYBOARD_KEYSTATEMANAGER_H

#include <memory>
#include "pico/stdlib.h"

struct KeyState {
    bool isPressed = false;
    uint32_t pressTime = 0;
    uint32_t releaseTime = 0;
    uint32_t totalPressCount = 0;
};

class KeyStateManager {
private:
    uint8_t numKeys;
    std::unique_ptr<KeyState[]> keyStates;
public:
    KeyStateManager(uint8_t numKeys);

    void updateKeyState(uint8_t keyIndex, bool isPressed, uint32_t timestamp);

    KeyState *getKeyState(uint8_t keyIndex);
};


#endif //PICOKEYBOARD_KEYSTATEMANAGER_H
