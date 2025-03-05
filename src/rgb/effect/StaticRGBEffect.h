#pragma once

#include "IRGBEffect.h"

class StaticRGBEffect final : public IRGBEffect {
    Color color;

    void serialize(InterDeviceCommunicator& communicator) override;

    Color getColor(LedConfig& led, absolute_time_t timestamp) override;

public:
    explicit StaticRGBEffect(const Color& color);

    explicit StaticRGBEffect(const InterDeviceCommunicator& communicator);
};
