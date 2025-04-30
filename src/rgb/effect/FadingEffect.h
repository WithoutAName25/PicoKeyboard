#pragma once
#include "IRGBEffect.h"

class FadingEffect final : public IRGBEffect {
    std::shared_ptr<IRGBEffect> oldEffect;
    std::shared_ptr<IRGBEffect> newEffect;
    absolute_time_t startTime;
    absolute_time_t endTime;

    void serialize(InterDeviceCommunicator& communicator) override;

    void enable(LedConfig* leds, LedConfig* mirroredLeds, uint8_t numLEDs) override;

    void disable() override;

    void update(absolute_time_t timestamp) override;

    Color getColor(LedConfig& led, absolute_time_t timestamp) override;

public:
    FadingEffect(const std::shared_ptr<IRGBEffect>& oldEffect, const std::shared_ptr<IRGBEffect>& newEffect,
                 absolute_time_t startTime, absolute_time_t endTime);

    explicit FadingEffect(InterDeviceCommunicator communicator);
};
