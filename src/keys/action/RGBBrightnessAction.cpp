#include "RGBBrightnessAction.h"

#include <rgb.h>

extern RGBController rgbController;

RGBBrightnessAction::RGBBrightnessAction(const float brightness, const bool absolute)
    : brightness(brightness), absolute(absolute) {}

void RGBBrightnessAction::execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) {
    rgbController.setBrightness(brightness, absolute);
}
