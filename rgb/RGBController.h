#ifndef PICOKEYBOARD_RGBCONTROLLER_H
#define PICOKEYBOARD_RGBCONTROLLER_H

#include "pico/stdlib.h"

class RGBController {
private:
    uint8_t numPixels;
    uint32_t* data;

public:
    explicit RGBController(uint8_t numPixels);
    ~RGBController();

    void write();
    void setPixel(uint8_t index, uint32_t colorGRBW);
    void setAll(uint32_t colorGRBW);
};

#endif //PICOKEYBOARD_RGBCONTROLLER_H