#pragma once

#include "PressReleaseAction.h"
#include "usb.h"

class ModifierAction : public PressReleaseAction {
private:
    Modifier modifier;

public:
    explicit ModifierAction(Modifier modifier);

    void press() override;

    void release() override;
};
