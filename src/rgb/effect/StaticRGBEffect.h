#pragma once

#include "IRGBEffect.h"

class StaticRGBEffect final : public IRGBEffect {
    uint32_t color;

    void serialize(InterDeviceCommunicator& communicator) override;

    uint32_t getColor(LedConfig& led, absolute_time_t timestamp) override;

public:
    explicit StaticRGBEffect(uint32_t colorGRBW);

    explicit StaticRGBEffect(InterDeviceCommunicator& communicator);
};
