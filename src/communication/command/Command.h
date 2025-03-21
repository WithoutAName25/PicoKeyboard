#pragma once

#include "pico/stdlib.h"
#include "../InterDeviceCommunicator.h"

// Command ids are prefix codes with a 1 at the start to mark the first bit
enum class CommonCommandIDs: uint16_t {
    KEY = 0b10
};

enum class PrimaryToSecondaryCommandIDs: uint16_t {
    RGB_EFFECT = 0b110000000
};

enum class SecondaryToPrimaryCommandIDs: uint16_t {};

class Command {
public:
    virtual ~Command() = default;

    /**
     * The first bits that are send must be the id without the first 1 bit
     */
    virtual void send(InterDeviceCommunicator& communicator) = 0;

    virtual void execute(absolute_time_t timestamp) = 0;
};
