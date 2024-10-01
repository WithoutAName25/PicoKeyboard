#pragma once

#include "IRGBEffect.h"

class StaticRGBEffect : public IRGBEffect {
private:
    uint32_t color;
public:
    explicit StaticRGBEffect(uint32_t colorGRBW);

    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};
