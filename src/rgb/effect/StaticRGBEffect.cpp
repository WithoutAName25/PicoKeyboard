#include "StaticRGBEffect.h"

StaticRGBEffect::StaticRGBEffect(uint32_t colorGRBW) {
    color = colorGRBW;
}

uint32_t StaticRGBEffect::getColor(LedConfig &led, absolute_time_t timestamp) {
    return color;
}
