#ifndef PICOKEYBOARD_IEXECUTABLE_H
#define PICOKEYBOARD_IEXECUTABLE_H

#include "pico/stdlib.h"

class IExecutable {
public:
    virtual ~IExecutable() = default;

    virtual void execute(absolute_time_t timestamp) = 0;
};

#endif //PICOKEYBOARD_IEXECUTABLE_H
