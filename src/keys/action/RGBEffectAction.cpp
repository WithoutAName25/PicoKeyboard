#include "RGBEffectAction.h"

#include <communication.h>

extern RGBController rgbController;

void RGBEffectAction::execute(uint8_t keyId, const KeyState *state, const absolute_time_t timestamp) {
    rgbController.setEffect(timestamp, effect, fadeTime);
}

RGBEffectAction::RGBEffectAction(const std::shared_ptr<IRGBEffect> &effect, const uint64_t fadeTime)
    : effect(effect), fadeTime(fadeTime) {}
