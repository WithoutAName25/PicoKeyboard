#pragma once

#include <map>

#include "IRGBEffect.h"
#include <util.h>

class HeatmapEffect final : public IRGBEffect, IExecutable {
    Color hot;
    Color cold;
    uint32_t coolingRate;
    uint32_t heatingOnPress;
    uint32_t heatingRateOnHold;
    bool mirrored;
    LedConfig* leds = nullptr;
    LedConfig* mirroredLeds = nullptr;
    uint8_t numLEDs = 0;
    std::map<uint8_t, uint8_t> inverseIdMap{};
    std::unique_ptr<uint64_t[]> heatMap = nullptr;
    uint64_t maxHeat = 0;
    Task* task = nullptr;
    absolute_time_t lastTick = 0;

    void serialize(InterDeviceCommunicator& communicator) override;

    Color getColor(LedConfig& led, absolute_time_t timestamp) override;

    void enable(LedConfig* leds, LedConfig* mirroredLeds, uint8_t numLEDs) override;

    void disable() override;

    void execute(absolute_time_t timestamp) override;

    [[nodiscard]] uint64_t getHeatDelta(uint8_t keyId) const;

public:
    HeatmapEffect(const Color& hot, const Color& cold, uint32_t coolingRate, uint32_t heatingOnPress,
                  uint32_t heatingRateOnHold,
                  bool mirrored);

    explicit HeatmapEffect(InterDeviceCommunicator& communicator);
};
