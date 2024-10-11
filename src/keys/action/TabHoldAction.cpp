#include "TabHoldAction.h"
#include "util.h"

extern Scheduler scheduler;
extern KeyStateController keyStateController;

void TabHoldAction::execute(absolute_time_t timestamp) {
    KeyState &currentState = keyStateController.getKeyState(activationKeyId);
    if (currentState.isPressed && currentState.totalPressCount == activationState.totalPressCount) {
        holdAction->execute(activationKeyId, &activationState, timestamp);
    }
}

void TabHoldAction::onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) {
    if (activationKeyId == keyId && !state.isPressed) {
        if (timestamp < activationTimestamp + tabTimeout) {
            tabAction->execute(activationKeyId, &activationState, timestamp);
        }
        keyStateController.removeKeyStateListener(listenerReference);
    }
}

void TabHoldAction::execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) {
    activationKeyId = keyId;
    activationState = *state;
    activationTimestamp = timestamp;
    scheduler.addTask(this, timestamp + holdTimeout);
    listenerReference = keyStateController.addKeyStateListener(this);
}
