#pragma once

#include "../LedConfig.h"

class IRGBEffect {
public:
    virtual ~IRGBEffect() = default;

    virtual void enable() {}

    virtual void disable() {}

    virtual void update(absolute_time_t timestamp) {}

    virtual uint32_t getColor(LedConfig &led, absolute_time_t timestamp) = 0;
};
