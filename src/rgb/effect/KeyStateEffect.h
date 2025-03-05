#pragma once

#include <map>
#include <memory>
#include "IRGBEffect.h"

class KeyStateEffect final : public IRGBEffect {
    std::shared_ptr<IRGBEffect> inactiveEffect;
    std::shared_ptr<IRGBEffect> activeEffect;
    uint64_t fadeOutMs;
    bool mirrored;
    std::map<uint8_t, uint8_t> mirroredKeyMap{};

    void serialize(InterDeviceCommunicator& communicator) override;

    [[nodiscard]] static uint64_t getTimeSinceRelease(uint8_t keyId, absolute_time_t timestamp);

    Color getColor(LedConfig& led, absolute_time_t timestamp) override;

    void enable(LedConfig* leds, LedConfig* mirroredLeds, uint8_t numLEDs) override;

public:
    template <typename T0, typename T1>
    KeyStateEffect(T0 inactiveEffect, T1 activeEffect, const uint64_t fadeOutMs, const bool mirrored)
        : IRGBEffect(EffectType::KEY_STATE),
          inactiveEffect(std::make_shared<T0>(std::move(inactiveEffect))),
          activeEffect(std::make_shared<T1>(std::move(activeEffect))),
          fadeOutMs(fadeOutMs),
          mirrored(mirrored) {}

    explicit KeyStateEffect(InterDeviceCommunicator& communicator);
};
