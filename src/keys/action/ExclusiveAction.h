#pragma once

#include <set>
#include <memory>
#include <vector>
#include "IPressReleaseAction.h"
#include "IKeyAction.h"
#include "util.h"

class [[maybe_unused]] ExclusiveAction : public IKeyAction, private IKeyStateListener, private IExecutable {
private:
    std::unique_ptr<IPressReleaseAction> action;
    std::set<uint8_t> keySet;

    uint8_t activatedBy = 0;
    KeyStateListenerReference listenerReference;
    absolute_time_t activationTimestamp = 0;

    void onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) override;

    void execute(absolute_time_t timestamp) override;

public:
    [[maybe_unused]] ExclusiveAction(std::unique_ptr<IPressReleaseAction> action,
                                     std::initializer_list<uint8_t> keySet);

    void execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) override;
};
