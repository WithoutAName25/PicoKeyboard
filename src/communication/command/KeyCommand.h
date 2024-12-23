#pragma once

#include "Command.h"
#include "../InterDeviceCommunicator.h"

class KeyCommand : public Command {
private:
    bool isPressed;
    uint8_t keyId;
public:
    static const uint16_t ID = 1;

    KeyCommand(bool isPressed, uint8_t keyId);

    explicit KeyCommand(InterDeviceCommunicator &communicator);

    void send(InterDeviceCommunicator &communicator) override;

    void execute(absolute_time_t timestamp) override;
};