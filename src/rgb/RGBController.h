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
    float brightness = 1.0f;

    void setPixel(uint8_t hwNumber, Color& color);

    void write() const;

    void execute(absolute_time_t timestamp) override;

public:
    RGBController(LedConfig* ledConfigs, LedConfig* mirroredLeds, uint8_t numLEDs);

    void setEffect(absolute_time_t timestamp, const std::shared_ptr<IRGBEffect>& effect, uint64_t fadeTime = 0,
                   bool sync = true);

    void setBrightness(float brightness, bool absolute, bool sync = true);
};
