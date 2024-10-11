#pragma once

#include <memory>
#include "IRGBEffect.h"

class KeyStateEffect : public IRGBEffect {
private:
    std::unique_ptr<IRGBEffect> colorEffect;
    uint64_t fadeOutMs;
public:
    template<typename T>
    explicit KeyStateEffect(T colorEffect,
                            uint64_t fadeOutMs)
            : colorEffect(std::make_unique<T>(std::move(colorEffect))),
              fadeOutMs(fadeOutMs) {}

    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};
