#include "RGBOverlayEffectCommand.h"

extern RGBController rgbController;

void RGBOverlayEffectCommand::execute(absolute_time_t timestamp) {
    if (effect != nullptr) {
        rgbController.addOverlayEffect(effect, false);
    } else {
        rgbController.removeOverlayEffect(indexToRemove);
    }
}

RGBOverlayEffectCommand::RGBOverlayEffectCommand(const std::shared_ptr<IRGBEffect>& effect)
    : effect(effect), indexToRemove(0) {}

RGBOverlayEffectCommand::RGBOverlayEffectCommand(const uint16_t indexToRemove)
    : effect(nullptr), indexToRemove(indexToRemove) {}

RGBOverlayEffectCommand::RGBOverlayEffectCommand(InterDeviceCommunicator& communicator) {
    effect = nullptr;
    indexToRemove = 0;

    if (communicator.receive() & 1) {
        effect = IRGBEffect::create(communicator);
    } else {
        indexToRemove = communicator.receive16();
    }
}

void RGBOverlayEffectCommand::send(InterDeviceCommunicator& communicator) {
    if (effect != nullptr) {
        communicator.send((ID << 1) & 0xFF | 1);
        effect->serialize(communicator);
    } else {
        communicator.send((ID << 1) & 0xFF);
        communicator.send16(indexToRemove);
    }
}
