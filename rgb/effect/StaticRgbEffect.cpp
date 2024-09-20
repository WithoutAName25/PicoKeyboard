#include "StaticRgbEffect.h"

StaticRgbEffect::StaticRgbEffect(uint32_t colorGRBW) {
    color = colorGRBW;
}

void StaticRgbEffect::start(RGBController &controller) {
    controller.setAll(color);
    controller.write();
}
