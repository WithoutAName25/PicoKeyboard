#pragma once

#include "../LedConfig.h"

class IRGBEffect {
public:
    virtual ~IRGBEffect() = default;

    virtual uint32_t getColor(LedConfig &led, absolute_time_t timestamp) = 0;
};
