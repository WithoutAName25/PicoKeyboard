#ifndef PICOKEYBOARD_RGB_H
#define PICOKEYBOARD_RGB_H

#include "hardware/pio.h"

void rgb_init(PIO pio, uint sm, uint pin);

void rgb_put_pixel(uint32_t pixel_grbw);

static inline uint32_t colorRGB(uint8_t red, uint8_t green, uint8_t blue) {
    return green << 24 | red << 16 | blue << 8;
}

static inline uint32_t colorRGBf(float red, float green, float blue) {
    return colorRGB((uint8_t) red * 255, (uint8_t) green * 255, (uint8_t) blue * 255);
}

uint32_t colorHSV(uint16_t hue, float saturation, float value);

#endif //PICOKEYBOARD_RGB_H
