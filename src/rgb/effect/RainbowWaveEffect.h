#pragma once

#include "IRGBEffect.h"

class RainbowWaveEffect final : public IRGBEffect {
public:
    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};
