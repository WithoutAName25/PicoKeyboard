#pragma once

#include "hardware/pio.h"

void rgb_init(PIO pio, uint pin);

void rgb_put_pixel(uint32_t pixel_grbw);

static uint32_t colorRGB(const uint8_t red, const uint8_t green, const uint8_t blue) {
    return green << 24 | red << 16 | blue << 8;
}

static uint32_t colorRGBW(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t white) {
    return green << 24 | red << 16 | blue << 8 | white;
}

static uint32_t colorRGBf(const float red, const float green, const float blue) {
    return colorRGB(static_cast<uint8_t>(red) * 255, static_cast<uint8_t>(green) * 255, static_cast<uint8_t>(blue) * 255);
}

static uint32_t colorRGBWf(const float red, const float green, const float blue, const float white) {
    return colorRGBW(static_cast<uint8_t>(red) * 255, static_cast<uint8_t>(green) * 255, static_cast<uint8_t>(blue) * 255, static_cast<uint8_t>(white) * 255);
}

uint32_t colorHSV(uint16_t hue, float saturation, float value);

uint32_t brightness(uint32_t color, float brightness);
