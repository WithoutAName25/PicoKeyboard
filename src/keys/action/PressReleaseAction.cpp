#include "PressReleaseAction.h"

extern KeyStateController keyStateController;

void PressReleaseAction::onKeyStateChange(const uint8_t keyId, const KeyState& state, const absolute_time_t timestamp) {
    if (activatedBy == keyId && !state.isPressed) {
        release(timestamp);
        keyStateController.removeKeyStateListener(listenerReference);
    }
}

void PressReleaseAction::execute(const uint8_t keyId, const KeyState* state, const absolute_time_t timestamp) {
    activatedBy = keyId;
    activationTimestamp = timestamp;
    press(timestamp);
    if (keyStateController.getKeyState(keyId).isPressed) {
        listenerReference = keyStateController.addKeyStateListener(this);
    } else {
        release(timestamp);
    }
}
