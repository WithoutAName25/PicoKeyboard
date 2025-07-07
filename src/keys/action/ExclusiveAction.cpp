#include "ExclusiveAction.h"

#include "util.h"

extern KeyStateController keyStateController;
extern Scheduler scheduler;

void ExclusiveAction::onKeyStateChange(const uint8_t keyId, const KeyState &state, const absolute_time_t timestamp) {
    if (activatedBy == keyId && !state.isPressed) {
        if (isActionActive) {
            action->release(timestamp);
            isActionActive = false;
        }
        keyStateController.removeKeyStateListener(listenerReference);
    } else if (keySet.contains(keyId)) {
        if (state.isPressed) {
            if (isActionActive) {
                action->release(timestamp);
                isActionActive = false;
            }
        } else if (!isActionActive) {
            for (const uint8_t key : keySet) {
                if (keyStateController.getKeyState(key).isPressed)
                    return;
            }
            action->press(timestamp);
            isActionActive = true;
        }
    }
}

void ExclusiveAction::execute(const uint8_t keyId, const KeyState *state, const absolute_time_t timestamp) {
    activatedBy = keyId;
    activationTimestamp = timestamp;
    action->press(timestamp);
    isActionActive = true;
    listenerReference = keyStateController.addKeyStateListener(this);
}
