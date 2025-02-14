#pragma once

#include <map>
#include <memory>
#include "IRGBEffect.h"

class KeyStateEffect final : public IRGBEffect {
    std::shared_ptr<IRGBEffect> colorEffect;
    uint64_t fadeOutMs;
    bool mirrored;
    std::map<uint8_t, uint8_t> mirroredKeyMap{};

    void serialize(InterDeviceCommunicator& communicator) override;

    [[nodiscard]] static uint64_t getTimeSinceRelease(uint8_t keyId, absolute_time_t timestamp);

    uint32_t getColor(LedConfig& led, absolute_time_t timestamp) override;

    void enable(LedConfig* leds, LedConfig* mirroredLeds, uint8_t numLEDs) override;

public:
    template <typename T>
    KeyStateEffect(T colorEffect, const uint64_t fadeOutMs, const bool mirrored)
        : IRGBEffect(EffectType::KEY_STATE), colorEffect(std::make_shared<T>(std::move(colorEffect))),
          fadeOutMs(fadeOutMs), mirrored(mirrored) {}

    explicit KeyStateEffect(InterDeviceCommunicator& communicator);
};
