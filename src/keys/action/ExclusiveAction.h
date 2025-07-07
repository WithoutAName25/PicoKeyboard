#pragma once

#include "IKeyAction.h"
#include "IPressReleaseAction.h"
#include <memory>
#include <set>
#include <vector>

class [[maybe_unused]] ExclusiveAction final : public IKeyAction, IKeyStateListener {
    std::unique_ptr<IPressReleaseAction> action;
    std::set<uint8_t> keySet;

    uint8_t activatedBy = 0;
    bool isActionActive = false;
    KeyStateListenerReference listenerReference;
    absolute_time_t activationTimestamp = 0;

    void onKeyStateChange(uint8_t keyId, const KeyState &state, absolute_time_t timestamp) override;

public:
    template <typename T>
    [[maybe_unused]] ExclusiveAction(T action, const std::initializer_list<uint8_t> keySet)
        : action(std::make_unique<T>(std::move(action))), keySet(keySet) {}

    void execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) override;
};
