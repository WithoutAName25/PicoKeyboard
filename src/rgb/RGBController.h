#pragma once

#include <memory>
#include "pico/stdlib.h"
#include "effect/IRGBEffect.h"
#include "../util/Scheduler.h"

class RGBController : public IExecutable {
private:
    LedConfig *ledConfigs;
    uint8_t numLEDs;
    std::unique_ptr<uint32_t[]> data;
    std::unique_ptr<IRGBEffect> currentEffect;

    void setPixel(uint8_t hwNumber, uint32_t colorGRBW);

    void write();

    void execute(absolute_time_t timestamp) override;

public:
    explicit RGBController(LedConfig *ledConfigs, uint8_t numLEDs);

    template<typename T>
    void setEffect(T effect) {
        currentEffect = std::make_unique<T>(std::move(effect));
    }
};
