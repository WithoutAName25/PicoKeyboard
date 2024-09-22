#ifndef PICOKEYBOARD_RGBCONTROLLER_H
#define PICOKEYBOARD_RGBCONTROLLER_H

#include <memory>
#include "pico/stdlib.h"
#include "effect/IRgbEffect.h"
#include "../util/Scheduler.h"

class RgbController : IExecutable {
private:
    LedConfig *ledConfigs;
    uint8_t numLEDs;
    std::unique_ptr<uint32_t[]> data;
    std::unique_ptr<IRgbEffect> currentEffect;

    void setPixel(uint8_t hwNumber, uint32_t colorGRBW);

    void write();

    void execute(absolute_time_t timestamp) override;

public:
    explicit RgbController(Scheduler &scheduler, LedConfig *ledConfigs, uint8_t numLEDs);

    void setEffect(std::unique_ptr<IRgbEffect> effect);
};

#endif //PICOKEYBOARD_RGBCONTROLLER_H