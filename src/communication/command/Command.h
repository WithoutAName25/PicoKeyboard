#pragma once

#include "pico/stdlib.h"
#include "../InterDeviceCommunicator.h"

class Command {
public:
    virtual ~Command() = default;

    /**
     * The first bits that are send must be the id without the first one bit
     */
    virtual void send(InterDeviceCommunicator &communicator) = 0;

    virtual void execute(absolute_time_t timestamp) = 0;
};
