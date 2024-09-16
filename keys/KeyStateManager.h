#ifndef PICOKEYBOARD_KEYSTATEMANAGER_H
#define PICOKEYBOARD_KEYSTATEMANAGER_H

#include <memory>
#include <functional>
#include "pico/stdlib.h"

struct KeyState {
    bool isPressed = false;
    uint32_t pressTime = 0;
    uint32_t releaseTime = 0;
    uint32_t totalPressCount = 0;
};

typedef std::function<void(uint8_t, KeyState *, uint32_t)> KeyStateListener;

class KeyStateManager {
private:
    uint8_t numKeys;
    std::unique_ptr<KeyState[]> keyStates;
    KeyStateListener *onStateChange;
public:
    KeyStateManager(uint8_t numKeys, KeyStateListener *onStateChange);

    void updateKeyState(uint8_t keyId, bool isPressed, uint32_t timestamp);

    KeyState *getKeyState(uint8_t keyId);
};


#endif //PICOKEYBOARD_KEYSTATEMANAGER_H
