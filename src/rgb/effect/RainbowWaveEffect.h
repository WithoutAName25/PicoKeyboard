#pragma once

#include "IRGBEffect.h"
#include "../../util/Task.h"

class RainbowWaveEffect : public IRGBEffect {
public:
    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};
