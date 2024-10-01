#pragma once

#include <memory>
#include "IRGBEffect.h"
#include "keys.h"

extern KeyStateController keyStateController;

class KeyStateEffect : public IRGBEffect {
private:
    std::unique_ptr<IRGBEffect> colorEffect;
    uint64_t fadeOutMs;
public:
    explicit KeyStateEffect(std::unique_ptr<IRGBEffect> colorEffect, uint64_t fadeOutMs);

    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};
