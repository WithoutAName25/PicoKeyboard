#pragma once

#include "PressReleaseAction.h"

class KeyAction : public PressReleaseAction {
private:
    uint8_t keycode;
public:
    explicit KeyAction(uint8_t keycode);

    void press() override;

    void release() override;
};
