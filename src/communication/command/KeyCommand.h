#pragma once

#include "Command.h"
#include "../InterDeviceCommunicator.h"

class KeyCommand final : public Command {
    bool isPressed;
    uint8_t keyId;

    void execute(absolute_time_t timestamp) override;

public:
    static constexpr uint16_t ID = static_cast<uint16_t>(CommonCommandIDs::KEY);

    KeyCommand(bool isPressed, uint8_t keyId);

    explicit KeyCommand(InterDeviceCommunicator& communicator);

    void send(InterDeviceCommunicator& communicator) override;
};
