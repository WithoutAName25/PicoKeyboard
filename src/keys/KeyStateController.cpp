#include "KeyStateController.h"

#include <algorithm>
#include <communication.h>

#define MIN_TIME_BETWEEN_STATE_CHANGES 2500

extern CommandController commandController;

ListenerPriority IKeyStateListener::getPriority() const {
    return ListenerPriority::WITH_KEYBOARD_CONTROLLER;
}

KeyStateController::KeyStateController(const uint8_t numKeys)
    : keyStates(new KeyState[numKeys]) {}

void KeyStateController::updateKeyState(const uint8_t keyId, const bool isPressed, const absolute_time_t timestamp,
                                        const bool localKey) {
    KeyState& state = keyStates[keyId];

    if (state.isPressed == isPressed) return;
    if (std::max(state.pressTime, state.releaseTime) + MIN_TIME_BETWEEN_STATE_CHANGES > timestamp) return;

    state.isPressed = isPressed;
    if (isPressed) {
        state.pressTime = timestamp;
        state.totalPressCount++;
    } else {
        state.releaseTime = timestamp;
    }

    if (localKey) commandController.send<KeyCommand>(isPressed, keyId);

    for (auto [priority, position] : removeQueue) {
        listeners[static_cast<int>(priority)].erase(position);
    }
    removeQueue.clear();
    for (int i = 0; i < static_cast<int>(ListenerPriority::Count); ++i) {
        for (IKeyStateListener* listener : listeners[i]) {
            listener->onKeyStateChange(keyId, state, timestamp);
        }
    }
}

KeyState& KeyStateController::getKeyState(const uint8_t keyId) {
    return keyStates[keyId];
}

KeyStateListenerReference KeyStateController::addKeyStateListener(IKeyStateListener* listener) {
    ListenerPriority priority = listener->getPriority();
    listeners[static_cast<int>(priority)].push_back(listener);
    return std::make_pair(priority, --listeners[static_cast<int>(priority)].end());
}

void KeyStateController::removeKeyStateListener(const KeyStateListenerReference& listenerReference) {
    removeQueue.push_back(listenerReference);
}
