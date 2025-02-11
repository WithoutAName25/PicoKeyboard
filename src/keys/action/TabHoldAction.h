#pragma once

#include <memory>
#include <usb.h>
#include <util.h>
#include "IKeyAction.h"
#include "KeyAction.h"
#include "ModifierAction.h"

class TabHoldAction final : public IKeyAction, IExecutable, IKeyStateListener {
    std::unique_ptr<IKeyAction> tabAction;
    std::unique_ptr<IKeyAction> holdAction;
    uint64_t tabTimeout;
    uint64_t holdTimeout;
    uint64_t tabHoldTimeout;
    bool holdOnPressRelease;
    bool blockOtherKeys;

    uint8_t activationKeyId = 0;
    KeyState activationState = {};
    absolute_time_t activationTimestamp = 0;

    bool isHoldActive = false;

    KeyStateListenerReference listenerReference;
    Task* task = nullptr;

    KeyBlockingReference blockingReference;
    bool areKeysBlocked = false;

    void execute(absolute_time_t timestamp) override;

    void onKeyStateChange(uint8_t keyId, const KeyState& state, absolute_time_t timestamp) override;

    TabHoldAction(std::unique_ptr<IKeyAction> tabAction,
                  std::unique_ptr<IKeyAction> holdAction,
                  uint64_t tabTimeout,
                  uint64_t holdTimeout,
                  uint64_t tabHoldTimeout,
                  bool holdOnPressRelease,
                  bool blockOtherKeys);

public:
    template <KeyActionType T0, KeyActionType T1>
    TabHoldAction(T0 tabAction, T1 holdAction, uint64_t tabTimeout, uint64_t holdTimeout, uint64_t tabHoldTimeout, bool holdOnPressRelease = false, bool blockOtherKeys = false)
        : TabHoldAction(std::make_unique<T0>(std::move(tabAction)),
                        std::make_unique<T1>(std::move(holdAction)),
                        tabTimeout,
                        holdTimeout,
                        tabHoldTimeout,
                        holdOnPressRelease,
                        blockOtherKeys) {}

    template <KeyActionType T0, KeyActionType T1>
    [[maybe_unused]] TabHoldAction(T0 tabAction, T1 holdAction, uint64_t timeout, uint64_t tabHoldTimeout)
        : TabHoldAction(tabAction, holdAction, timeout, timeout, tabHoldTimeout) {}

    template <KeyActionType T0, KeyActionType T1>
    [[maybe_unused]] TabHoldAction(T0 tabAction, T1 holdAction, uint64_t timeout)
        : TabHoldAction(tabAction, holdAction, timeout, timeout, timeout) {}

    TabHoldAction(uint8_t keycode, Modifier modifier, const uint64_t timeout, const uint64_t tabHoldTimeout)
        : TabHoldAction(std::make_unique<KeyAction>(keycode),
                        std::make_unique<ModifierAction>(modifier),
                        timeout,
                        timeout,
                        tabHoldTimeout,
                        true,
                        true) {}

    void execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) override;
};
