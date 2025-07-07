#include "TabHoldAction.h"

#include <rgb.h>
#include <sys/stat.h>
#include <util.h>

extern Scheduler scheduler;
extern KeyStateController keyStateController;
extern HIDKeyboard hidKeyboard;
extern RGBController rgbController;

void TabHoldAction::execute(absolute_time_t timestamp) {
    if (areKeysBlocked) {
        hidKeyboard.unblockNewKeys(blockingReference);
        areKeysBlocked = false;
    }
    if (const KeyState &currentState = keyStateController.getKeyState(activationKeyId);
        !isHoldActive && currentState.isPressed && currentState.totalPressCount == activationState.totalPressCount) {
        holdAction->execute(activationKeyId, &activationState, activationTimestamp);
        isHoldActive = true;
    }
    task = nullptr;
}

void TabHoldAction::onKeyStateChange(const uint8_t keyId, const KeyState &state, const absolute_time_t timestamp) {
    if (activationKeyId == keyId) {
        if (state.isPressed)
            return;

        if (timestamp < activationTimestamp + tabTimeout) {
            tabAction->execute(activationKeyId, &activationState, activationTimestamp);
        }
        keyStateController.removeKeyStateListener(listenerReference);
        if (task != nullptr) {
            task->cancel();
            task = nullptr;
        }
        if (areKeysBlocked) {
            task = scheduler.addTask(this, timestamp + 10000);
        }
    } else if (!isHoldActive && holdOnPressRelease && !state.isPressed && state.pressTime > activationTimestamp) {
        if (const KeyState &currentState = keyStateController.getKeyState(activationKeyId);
            currentState.isPressed && currentState.totalPressCount == activationState.totalPressCount) {
            holdAction->execute(activationKeyId, &activationState, activationTimestamp);
            isHoldActive = true;
        }
        keyStateController.removeKeyStateListener(listenerReference);
        if (task != nullptr) {
            task->cancel();
            task = nullptr;
        }
        if (areKeysBlocked) {
            hidKeyboard.unblockNewKeys(blockingReference);
            areKeysBlocked = false;
        }
    }
}

TabHoldAction::TabHoldAction(std::unique_ptr<IKeyAction> tabAction, std::unique_ptr<IKeyAction> holdAction,
                             const uint64_t tabTimeout, const uint64_t holdTimeout, const uint64_t tabHoldTimeout,
                             const bool holdOnPressRelease, const bool blockOtherKeys)
    : tabAction(std::move(tabAction)), holdAction(std::move(holdAction)), tabTimeout(tabTimeout),
      holdTimeout(holdTimeout), tabHoldTimeout(tabHoldTimeout), holdOnPressRelease(holdOnPressRelease),
      blockOtherKeys(blockOtherKeys) {}

void TabHoldAction::execute(const uint8_t keyId, const KeyState *state, const absolute_time_t timestamp) {
    if (task != nullptr) {
        task->cancel();
        task = nullptr;
    }
    if (areKeysBlocked) {
        hidKeyboard.unblockNewKeys(blockingReference);
        areKeysBlocked = false;
    }
    isHoldActive = false;
    if (timestamp < activationTimestamp + tabHoldTimeout) {
        tabAction->execute(keyId, state, timestamp);
    } else {
        if (blockOtherKeys) {
            blockingReference = hidKeyboard.blockNewKeys(timestamp);
            areKeysBlocked = true;
        }
        task = scheduler.addTask(this, timestamp + holdTimeout);
        listenerReference = keyStateController.addKeyStateListener(this);
    }
    activationKeyId = keyId;
    activationState = *state;
    activationTimestamp = timestamp;
}
