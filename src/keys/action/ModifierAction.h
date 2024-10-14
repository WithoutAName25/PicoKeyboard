#pragma once

#include "PressReleaseAction.h"
#include "usb.h"

class [[maybe_unused]] ModifierAction : public PressReleaseAction {
private:
    Modifier modifier;

public:
    [[maybe_unused]] explicit ModifierAction(Modifier modifier);

    void press() override;

    void release() override;
};
