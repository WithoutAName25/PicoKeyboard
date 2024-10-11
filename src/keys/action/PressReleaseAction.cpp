#include "PressReleaseAction.h"

extern KeyStateController keyStateController;
extern Scheduler scheduler;

const uint64_t MIN_HOLD_TIME_US = 10000;

void PressReleaseAction::onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) {
    if (activatedBy == keyId && !state.isPressed) {
        if (timestamp < activationTimestamp + MIN_HOLD_TIME_US) {
            scheduler.addTask(this, activationTimestamp + MIN_HOLD_TIME_US);
        } else {
            release();
        }
        keyStateController.removeKeyStateListener(listenerReference);
    }
}

void PressReleaseAction::execute(absolute_time_t timestamp) {
    release();
}

void PressReleaseAction::execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) {
    activatedBy = keyId;
    activationTimestamp = timestamp;
    press();
    listenerReference = keyStateController.addKeyStateListener(this);
}
