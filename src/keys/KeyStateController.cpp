
#include "KeyStateController.h"

KeyStateController::KeyStateController(uint8_t numKeys)
        : numKeys(numKeys), keyStates(new KeyState[numKeys]), listeners() {
}

void KeyStateController::updateKeyState(uint8_t keyId, bool isPressed, absolute_time_t timestamp) {
    KeyState &state = keyStates[keyId];
    if (state.isPressed == isPressed) return;
    state.isPressed = isPressed;
    if (isPressed) {
        state.pressTime = timestamp;
        state.totalPressCount++;
    } else {
        state.releaseTime = timestamp;
    }
    for (int i = 0; i < static_cast<int>(ListenerPriority::Count); ++i) {
        for (IKeyStateListener *listener: listeners[i]) {
            listener->onKeyStateChange(keyId, state, timestamp);
        }
    }
}

KeyState &KeyStateController::getKeyState(uint8_t keyId) {
    return keyStates[keyId];
}

KeyStateListenerReference KeyStateController::addKeyStateListener(IKeyStateListener *listener) {
    ListenerPriority priority = listener->getPriority();
    listeners[static_cast<int>(priority)].push_back(listener);
    return std::make_pair(priority, --listeners[static_cast<int>(priority)].end());
}

void KeyStateController::removeKeyStateListener(KeyStateListenerReference listenerReference) {
    listeners[static_cast<int>(listenerReference.first)].erase(listenerReference.second);
}
