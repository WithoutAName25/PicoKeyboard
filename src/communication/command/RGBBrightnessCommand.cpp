#include "RGBBrightnessCommand.h"

#include <rgb.h>

extern RGBController rgbController;

void RGBBrightnessCommand::execute(absolute_time_t timestamp) {
    rgbController.setBrightness(brightness, absolute, false);
}

RGBBrightnessCommand::RGBBrightnessCommand(const float brightness, const bool absolute)
    : brightness(brightness), absolute(absolute) {}

RGBBrightnessCommand::RGBBrightnessCommand(InterDeviceCommunicator &communicator) {
    communicator.receive();
    brightness = 0;
    communicator.receive(reinterpret_cast<uint8_t *>(&brightness), sizeof(brightness));
    absolute = communicator.receive();
}

void RGBBrightnessCommand::send(InterDeviceCommunicator &communicator) {
    communicator.send(ID & 0xFF);
    communicator.send(reinterpret_cast<uint8_t *>(&brightness), sizeof(brightness));
    communicator.send(absolute);
}
