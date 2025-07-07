#pragma once

#include "../InterDeviceCommunicator.h"
#include "pico/stdlib.h"

// Command ids are prefix codes with a 1 at the start to mark the first bit
enum class CommonCommandIDs : uint16_t {
    KEY = 0b10,
};

enum class PrimaryToSecondaryCommandIDs : uint16_t {
    RGB_EFFECT = 0b110000000,
    RGB_BRIGHTNESS = 0b110000001,
    RGB_OVERLAY_EFFECT = 0b11000001,
};

enum class SecondaryToPrimaryCommandIDs : uint16_t {};

class Command {
public:
    virtual ~Command() = default;

    /**
     * The first bits that are send must be the id without the first 1 bit
     */
    virtual void send(InterDeviceCommunicator &communicator) = 0;

    virtual void execute(absolute_time_t timestamp) = 0;
};
