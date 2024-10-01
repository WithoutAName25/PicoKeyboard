#include "RainbowWaveEffect.h"
#include "../neopixel.h"

uint32_t RainbowWaveEffect::getColor(LedConfig &led, absolute_time_t timestamp) {
    return colorHSV(((255 - led.id) * 10 + (timestamp >> 14)) % 360, 1, 1);
}
