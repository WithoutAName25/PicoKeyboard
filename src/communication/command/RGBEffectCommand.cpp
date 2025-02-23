#include "RGBEffectCommand.h"

extern RGBController rgbController;

void RGBEffectCommand::execute(absolute_time_t timestamp) {
    rgbController.setEffect(effect);
}

RGBEffectCommand::RGBEffectCommand(const std::shared_ptr<IRGBEffect>& effect): effect(effect) {}

RGBEffectCommand::RGBEffectCommand(InterDeviceCommunicator& communicator) {
    communicator.receive();
    effect = IRGBEffect::create(communicator);
}

void RGBEffectCommand::send(InterDeviceCommunicator& communicator) {
    communicator.send(ID & 0xFF);
    effect->serialize(communicator);
}
