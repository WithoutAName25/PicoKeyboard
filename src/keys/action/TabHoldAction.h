#pragma once

#include <memory>
#include "IKeyAction.h"
#include "util.h"
#include "PressReleaseAction.h"

class TabHoldAction : public IKeyAction, private IExecutable, private IKeyStateListener {
private:
    std::unique_ptr<IKeyAction> tabAction;
    std::unique_ptr<IKeyAction> holdAction;
    uint64_t tabTimeout;
    uint64_t holdTimeout;
    uint64_t tabHoldTimeout;

    uint8_t activationKeyId = 0;
    KeyState activationState = {};
    absolute_time_t activationTimestamp = 0;

    KeyStateListenerReference listenerReference;
    Task *task = nullptr;

    void execute(absolute_time_t timestamp) override;

    void onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) override;

public:
    template<typename T0, typename T1>
    TabHoldAction(T0 tabAction, T1 holdAction, uint64_t tabTimeout, uint64_t holdTimeout, uint64_t tabHoldTimeout)
            : tabAction(std::make_unique<T0>(std::move(tabAction))),
              holdAction(std::make_unique<T1>(std::move(holdAction))),
              tabTimeout(tabTimeout),
              holdTimeout(holdTimeout),
              tabHoldTimeout(tabHoldTimeout) {}

    template<typename T0, typename T1>
    [[maybe_unused]] TabHoldAction(T0 tabAction, T1 holdAction, uint64_t timeout, uint64_t tabHoldTimeout)
            : TabHoldAction(tabAction, holdAction, timeout, timeout, tabHoldTimeout) {}

    template<typename T0, typename T1>
    [[maybe_unused]] TabHoldAction(T0 tabAction, T1 holdAction, uint64_t timeout)
            : TabHoldAction(tabAction, holdAction, timeout, timeout, timeout) {}

    void execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) override;
};
