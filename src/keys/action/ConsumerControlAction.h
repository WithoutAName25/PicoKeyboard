#pragma once
#include "IKeyAction.h"

class ConsumerControlAction : public IKeyAction {
    uint16_t command;
public:
    explicit ConsumerControlAction(uint16_t command);

    void execute(uint8_t keyId, KeyState* state, absolute_time_t timestamp) override;
};
