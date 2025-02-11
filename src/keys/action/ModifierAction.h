#pragma once

#include "PressReleaseAction.h"
#include "usb.h"

class [[maybe_unused]] ModifierAction final : public PressReleaseAction {
    Modifier modifier;

public:
    [[maybe_unused]] explicit ModifierAction(Modifier modifier);

    void press(absolute_time_t timestamp) override;

    void release(absolute_time_t timestamp) override;
};
