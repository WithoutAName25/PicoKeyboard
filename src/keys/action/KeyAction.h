#pragma once

#include "PressReleaseAction.h"

class KeyAction : public PressReleaseAction {
private:
    uint8_t keycode;
public:
    explicit KeyAction(uint8_t keycode);

    void press(absolute_time_t timestamp) override;

    void release(absolute_time_t timestamp) override;
};
