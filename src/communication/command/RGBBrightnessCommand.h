#pragma once
#include "Command.h"

class RGBBrightnessCommand final : public Command {
    float brightness;
    bool absolute;

    void execute(absolute_time_t timestamp) override;

public:
    static constexpr uint16_t ID = static_cast<uint16_t>(PrimaryToSecondaryCommandIDs::RGB_BRIGHTNESS);

    explicit RGBBrightnessCommand(float brightness, bool absolute = true);

    explicit RGBBrightnessCommand(InterDeviceCommunicator &communicator);

    void send(InterDeviceCommunicator &communicator) override;
};
