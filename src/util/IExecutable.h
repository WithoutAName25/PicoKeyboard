#pragma once

#include "pico/stdlib.h"

class IExecutable {
public:
    virtual ~IExecutable() = default;

    virtual void execute(absolute_time_t timestamp) = 0;
};
