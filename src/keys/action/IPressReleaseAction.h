#pragma once

#include "pico/stdlib.h"

class IPressReleaseAction {
public:
    virtual ~IPressReleaseAction() = default;

    virtual void press(absolute_time_t timestamp) = 0;

    virtual void release(absolute_time_t timestamp) = 0;
};
