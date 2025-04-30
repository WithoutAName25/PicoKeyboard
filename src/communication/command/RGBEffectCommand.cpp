#include "RGBEffectCommand.h"

extern RGBController rgbController;

void RGBEffectCommand::execute(const absolute_time_t timestamp) {
    rgbController.setEffect(timestamp, effect, fadeTime, false);
}

RGBEffectCommand::RGBEffectCommand(const std::shared_ptr<IRGBEffect>& effect, uint64_t fadeTime)
    : effect(effect), fadeTime(fadeTime) {}

RGBEffectCommand::RGBEffectCommand(InterDeviceCommunicator& communicator) {
    communicator.receive();
    effect = IRGBEffect::create(communicator);
    fadeTime = communicator.receive64();
}

void RGBEffectCommand::send(InterDeviceCommunicator& communicator) {
    communicator.send(ID & 0xFF);
    effect->serialize(communicator);
    communicator.send64(fadeTime);
}
