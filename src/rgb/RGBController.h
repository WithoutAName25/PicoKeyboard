#pragma once

#include <memory>
#include <util.h>

#include "Color.h"
#include "pico/stdlib.h"
#include "effect/IRGBEffect.h"

class RGBController final : public IExecutable {
    LedConfig* ledConfigs;
    LedConfig* mirroredLeds;
    uint8_t numLEDs;
    std::unique_ptr<uint32_t[]> data;
    std::shared_ptr<IRGBEffect> currentEffect;

    void setPixel(uint8_t hwNumber, Color &color);

    void write() const;

    void execute(absolute_time_t timestamp) override;

public:
    RGBController(LedConfig* ledConfigs, LedConfig* mirroredLeds, uint8_t numLEDs);

    void setEffect(const std::shared_ptr<IRGBEffect>& effect);
};
