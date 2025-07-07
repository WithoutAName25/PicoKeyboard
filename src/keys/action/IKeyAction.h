#pragma once

#include "../KeyStateController.h"
#include "pico/stdlib.h"

class IKeyAction {
public:
    virtual ~IKeyAction() = default;

    virtual void execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) = 0;
};

template <typename T>
concept KeyActionType = std::is_base_of_v<IKeyAction, T>;
