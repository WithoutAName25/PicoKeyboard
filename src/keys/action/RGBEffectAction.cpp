#include "RGBEffectAction.h"

#include <communication.h>

extern RGBController rgbController;
extern CommandController commandController;

void RGBEffectAction::execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) {
    rgbController.setEffect(effect);
    commandController.send<RGBEffectCommand>(effect);
}

RGBEffectAction::RGBEffectAction(const std::shared_ptr<IRGBEffect>& effect) : effect(effect) {}
