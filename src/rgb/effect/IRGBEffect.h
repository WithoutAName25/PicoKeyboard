#pragma once

#include <memory>
#include <pico/stdlib.h>
#include "../LedConfig.h"

class InterDeviceCommunicator;

enum class EffectType: uint8_t {
    KEY_STATE,
    RAINBOW_WAVE,
    STATIC_RGB
};

class IRGBEffect {
protected:
    EffectType type;

public:
    explicit IRGBEffect(const EffectType type) : type(type) {}

    virtual ~IRGBEffect() = default;

    virtual void serialize(InterDeviceCommunicator& communicator);

    static std::shared_ptr<IRGBEffect> create(InterDeviceCommunicator& communicator);

    virtual void enable(LedConfig* leds, LedConfig* mirroredLeds, uint8_t numLEDs) {}

    virtual void disable() {}

    virtual void update(absolute_time_t timestamp) {}

    virtual uint32_t getColor(LedConfig& led, absolute_time_t timestamp) = 0;
};

template <typename T>
concept RGBEffectType = std::is_base_of_v<IRGBEffect, T>;
