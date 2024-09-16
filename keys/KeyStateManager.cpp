
#include "KeyStateManager.h"

KeyStateManager::KeyStateManager(uint8_t numKeys, KeyStateListener *onStateChange)
        : numKeys(numKeys), keyStates(new KeyState[numKeys]), onStateChange(onStateChange) {
}

void KeyStateManager::updateKeyState(uint8_t keyId, bool isPressed, uint32_t timestamp) {
    KeyState &state = keyStates[keyId];
    if (state.isPressed == isPressed) return;
    state.isPressed = isPressed;
    if (isPressed) {
        state.pressTime = timestamp;
        state.totalPressCount++;
    } else {
        state.releaseTime = timestamp;
    }
    (*onStateChange)(keyId, &state, timestamp);
}

KeyState *KeyStateManager::getKeyState(uint8_t keyId) {
    return &keyStates[keyId];
}
