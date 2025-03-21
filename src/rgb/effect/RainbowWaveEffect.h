#pragma once

#include "IRGBEffect.h"

class RainbowWaveEffect final : public IRGBEffect {
    void serialize(InterDeviceCommunicator& communicator) override;

    Color getColor(LedConfig& led, absolute_time_t timestamp) override;

public:
    RainbowWaveEffect();

    explicit RainbowWaveEffect(InterDeviceCommunicator& communicator);
};
