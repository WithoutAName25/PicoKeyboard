#pragma once
#include "IKeyAction.h"

class RGBBrightnessAction : public IKeyAction {
    float brightness;
    bool absolute;

public:
    explicit RGBBrightnessAction(float brightness, bool absolute = true);

    void execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) override;
};
