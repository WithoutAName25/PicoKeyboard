#include "StaticRgbEffect.h"

StaticRgbEffect::StaticRgbEffect(uint32_t colorGRBW) {
    color = colorGRBW;
}

uint32_t StaticRgbEffect::getColor(LedConfig &led, absolute_time_t timestamp) {
    return color;
}
