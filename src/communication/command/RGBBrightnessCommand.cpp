#include "RGBBrightnessCommand.h"

#include <rgb.h>

extern RGBController rgbController;

void RGBBrightnessCommand::execute(absolute_time_t timestamp) {
    rgbController.setBrightness(brightness, absolute, false);
}

RGBBrightnessCommand::RGBBrightnessCommand(float brightness, bool absolute)
    : brightness(brightness), absolute(absolute) {}

RGBBrightnessCommand::RGBBrightnessCommand(InterDeviceCommunicator& communicator) {
    communicator.receive();
    brightness = std::bit_cast<float>(communicator.receive32());
    absolute = communicator.receive();
}

void RGBBrightnessCommand::send(InterDeviceCommunicator& communicator) {
    communicator.send(ID & 0xFF);
    communicator.send32(std::bit_cast<uint32_t>(brightness));
    communicator.send(absolute);
}
