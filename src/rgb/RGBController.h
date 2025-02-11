#pragma once

#include <memory>
#include "pico/stdlib.h"
#include "effect/IRGBEffect.h"
#include "../util/Scheduler.h"

class RGBController final : public IExecutable {
    LedConfig *ledConfigs;
    uint8_t numLEDs;
    std::unique_ptr<uint32_t[]> data;
    IRGBEffect* currentEffect;

    void setPixel(uint8_t hwNumber, uint32_t colorGRBW);

    void write() const;

    void execute(absolute_time_t timestamp) override;

public:
    explicit RGBController(LedConfig *ledConfigs, uint8_t numLEDs);

    void setEffect(IRGBEffect* effect);
};
