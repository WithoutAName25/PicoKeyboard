#include "TabHoldAction.h"
#include "util.h"

extern Scheduler scheduler;
extern KeyStateController keyStateController;
extern HIDKeyboard hidKeyboard;

void TabHoldAction::execute(absolute_time_t timestamp) {
    if (areKeysBlocked) {
        hidKeyboard.unblockNewKeys(blockingReference);
        areKeysBlocked = false;
    }
    KeyState& currentState = keyStateController.getKeyState(activationKeyId);
    if (currentState.isPressed && currentState.totalPressCount == activationState.totalPressCount) {
        holdAction->execute(activationKeyId, &activationState, activationTimestamp);
    }
    task = nullptr;
}

void TabHoldAction::onKeyStateChange(uint8_t keyId, KeyState& state, absolute_time_t timestamp) {
    if (activationKeyId == keyId && !state.isPressed) {
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
    }
}

TabHoldAction::TabHoldAction(std::unique_ptr<IKeyAction> tabAction,
                             std::unique_ptr<IKeyAction> holdAction,
                             uint64_t tabTimeout,
                             uint64_t holdTimeout,
                             uint64_t tabHoldTimeout,
                             bool blockOtherKeys)
    : tabAction(std::move(tabAction)),
      holdAction(std::move(holdAction)),
      tabTimeout(tabTimeout),
      holdTimeout(holdTimeout),
      tabHoldTimeout(tabHoldTimeout),
      blockOtherKeys(blockOtherKeys) {}

void TabHoldAction::execute(uint8_t keyId, KeyState* state, absolute_time_t timestamp) {
    if (task != nullptr) {
        task->cancel();
        task = nullptr;
    }
    if (areKeysBlocked) {
        hidKeyboard.unblockNewKeys(blockingReference);
        areKeysBlocked = false;
    }
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
