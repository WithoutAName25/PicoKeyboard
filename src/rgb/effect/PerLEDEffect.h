#pragma once
#include <cstdint>
#include <map>
#include <set>

#include "IRGBEffect.h"

class PerLEDEffect final : public IRGBEffect {
    std::map<uint8_t, std::shared_ptr<IRGBEffect>> effectPerLED;
    std::map<std::shared_ptr<IRGBEffect>, uint8_t> effectToId;
    std::map<uint8_t, std::shared_ptr<IRGBEffect>> idToEffect;
    std::shared_ptr<IRGBEffect> fallback;

    void serialize(InterDeviceCommunicator& communicator) override;

    Color getColor(LedConfig& led, absolute_time_t timestamp) override;

    void enable(LedConfig* leds, LedConfig* mirroredLeds, uint8_t numLEDs) override;

    void disable() override;

public:
    PerLEDEffect(std::initializer_list<std::pair<const uint8_t, std::shared_ptr<IRGBEffect>>> effectPerLED,
                 std::shared_ptr<IRGBEffect> fallback = nullptr);

    explicit PerLEDEffect(InterDeviceCommunicator& communicator);
};
