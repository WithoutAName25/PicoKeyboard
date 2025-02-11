#include "KeyStateController.h"

ListenerPriority IKeyStateListener::getPriority() const {
    return ListenerPriority::WITH_KEYBOARD_CONTROLLER;
}

KeyStateController::KeyStateController(const uint8_t numKeys)
        : keyStates(new KeyState[numKeys]) {
}

void KeyStateController::updateKeyState(const uint8_t keyId, const bool isPressed, const absolute_time_t timestamp) {
    KeyState &state = keyStates[keyId];
    if (state.isPressed == isPressed) return;
    state.isPressed = isPressed;
    if (isPressed) {
        state.pressTime = timestamp;
        state.totalPressCount++;
    } else {
        state.releaseTime = timestamp;
    }
    for (auto [priority, position]: removeQueue) {
        listeners[static_cast<int>(priority)].erase(position);
    }
    removeQueue.clear();
    for (int i = 0; i < static_cast<int>(ListenerPriority::Count); ++i) {
        for (IKeyStateListener *listener: listeners[i]) {
            listener->onKeyStateChange(keyId, state, timestamp);
        }
    }
}

KeyState &KeyStateController::getKeyState(const uint8_t keyId) {
    return keyStates[keyId];
}

KeyStateListenerReference KeyStateController::addKeyStateListener(IKeyStateListener *listener) {
    ListenerPriority priority = listener->getPriority();
    listeners[static_cast<int>(priority)].push_back(listener);
    return std::make_pair(priority, --listeners[static_cast<int>(priority)].end());
}

void KeyStateController::removeKeyStateListener(const KeyStateListenerReference& listenerReference) {
    removeQueue.push_back(listenerReference);
}
