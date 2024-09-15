
#include "KeyStateManager.h"

KeyStateManager::KeyStateManager(uint8_t numKeys)
        : numKeys(numKeys), keyStates(new KeyState[numKeys]) {
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
}

KeyState *KeyStateManager::getKeyState(uint8_t keyIndex) {
    return &keyStates.get()[keyIndex];
}
