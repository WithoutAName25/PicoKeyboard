#pragma once

#include <memory>
#include <util.h>

#include "Color.h"
#include "effect/IRGBEffect.h"
#include "pico/stdlib.h"

using OverlayEffectReference = std::list<std::shared_ptr<IRGBEffect>>::iterator;

class RGBController final : public IExecutable {
    LedConfig *ledConfigs;
    LedConfig *mirroredLeds;
    uint8_t numLEDs;
    std::unique_ptr<uint32_t[]> data;
    std::shared_ptr<IRGBEffect> currentEffect;
    std::list<std::shared_ptr<IRGBEffect>> overlayEffects;
    float brightness = 1.0f;

    void setPixel(uint8_t hwNumber, Color &color);

    void write() const;
    Color getColor(absolute_time_t timestamp, LedConfig &ledConfig) const;

    void execute(absolute_time_t timestamp) override;

public:
    RGBController(LedConfig *ledConfigs, LedConfig *mirroredLeds, uint8_t numLEDs);

    void setEffect(absolute_time_t timestamp, const std::shared_ptr<IRGBEffect> &effect, uint64_t fadeTime = 0,
                   bool sync = true);

    void setBrightness(float brightness, bool absolute = true, bool sync = true);

    OverlayEffectReference addOverlayEffect(const std::shared_ptr<IRGBEffect> &effect, bool sync = true);

    void removeOverlayEffect(OverlayEffectReference reference);

    void removeOverlayEffect(uint16_t id);
};
