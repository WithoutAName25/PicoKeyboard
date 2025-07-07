#pragma once
#include "IKeyAction.h"

class BootloaderAction final : public IKeyAction {
public:
    void execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) override;
};
