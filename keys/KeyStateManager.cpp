
#include "KeyStateManager.h"

KeyStateManager::KeyStateManager(uint8_t numKeys, KeyStateListener *onStateChange)
        : numKeys(numKeys), keyStates(new KeyState[numKeys]), onStateChange(onStateChange) {
}

void KeyStateManager::updateKeyState(uint8_t keyIndex, bool isPressed, uint32_t timestamp) {
    KeyState &state = keyStates.get()[keyIndex];
    if (state.isPressed == isPressed) return;
    state.isPressed = isPressed;
    if (isPressed) {
        state.pressTime = timestamp;
        state.totalPressCount++;
    } else {
        state.releaseTime = timestamp;
    }
    (*onStateChange)(keyIndex, &state, timestamp);
}

KeyState *KeyStateManager::getKeyState(uint8_t keyIndex) {
    return &keyStates.get()[keyIndex];
}
