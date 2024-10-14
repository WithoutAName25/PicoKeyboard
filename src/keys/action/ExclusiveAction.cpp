#include "ExclusiveAction.h"

extern KeyStateController keyStateController;
extern Scheduler scheduler;

const uint64_t MIN_HOLD_TIME_US = 10000;

void ExclusiveAction::onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) {
    if (activatedBy == keyId && !state.isPressed) {
        if (timestamp < activationTimestamp + MIN_HOLD_TIME_US) {
            scheduler.addTask(this, activationTimestamp + MIN_HOLD_TIME_US);
        } else {
            action->release();
        }
        keyStateController.removeKeyStateListener(listenerReference);
    } else if (keySet.contains(keyId) && state.isPressed) {
        action->release();
        keyStateController.removeKeyStateListener(listenerReference);
    }
}

void ExclusiveAction::execute(absolute_time_t timestamp) {
    action->release();
}

[[maybe_unused]] [[maybe_unused]] [[maybe_unused]] ExclusiveAction::ExclusiveAction(
        std::unique_ptr<IPressReleaseAction> action, std::initializer_list<uint8_t> keySet)
        : action(std::move(action)), keySet(keySet) {}

void ExclusiveAction::execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) {
    activatedBy = keyId;
    activationTimestamp = timestamp;
    action->press();
    listenerReference = keyStateController.addKeyStateListener(this);
}
