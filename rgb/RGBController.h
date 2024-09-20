#ifndef PICOKEYBOARD_RGBCONTROLLER_H
#define PICOKEYBOARD_RGBCONTROLLER_H

#include <memory>
#include "pico/stdlib.h"
#include "effect/IRgbEffect.h"

class IRgbEffect;

class RGBController {
private:
    uint8_t numPixels;
    std::unique_ptr<uint32_t[]> data;
    std::unique_ptr<IRgbEffect> currentEffect;

public:
    explicit RGBController(uint8_t numPixels);

    void write();

    void setPixel(uint8_t index, uint32_t colorGRBW);

    void setAll(uint32_t colorGRBW);

    void setEffect(std::unique_ptr<IRgbEffect> effect);
};

#endif //PICOKEYBOARD_RGBCONTROLLER_H