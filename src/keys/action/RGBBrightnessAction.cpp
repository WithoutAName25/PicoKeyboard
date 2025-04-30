#include "RGBBrightnessAction.h"

#include <rgb.h>

extern RGBController rgbController;

inline RGBBrightnessAction::RGBBrightnessAction(const float brightness, const bool absolute)
    : brightness(brightness), absolute(absolute) {}

void RGBBrightnessAction::execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) {
    if (absolute) {
        rgbController.setBrightness(brightness);
    } else {
        rgbController.incrementBrightness(brightness);
    }
}
