#include "ExclusiveAction.h"

extern KeyStateController keyStateController;
extern Scheduler scheduler;

const uint64_t MIN_HOLD_TIME_US = 10000;

void ExclusiveAction::onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) {
    if (activatedBy == keyId && !state.isPressed) {
        if (timestamp < activationTimestamp + MIN_HOLD_TIME_US) {
            scheduler.addTask(this, activationTimestamp + MIN_HOLD_TIME_US);
        } else {
            action->release(timestamp);
        }
        keyStateController.removeKeyStateListener(listenerReference);
    } else if (keySet.contains(keyId) && state.isPressed) {
        action->release(timestamp);
        keyStateController.removeKeyStateListener(listenerReference);
    }
}

void ExclusiveAction::execute(absolute_time_t timestamp) {
    action->release(timestamp);
}

void ExclusiveAction::execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) {
    activatedBy = keyId;
    activationTimestamp = timestamp;
    action->press(timestamp);
    listenerReference = keyStateController.addKeyStateListener(this);
}
