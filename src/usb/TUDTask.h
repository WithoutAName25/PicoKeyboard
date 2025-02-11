#pragma once

#include "../util/IExecutable.h"
#include "pico/stdlib.h"

class TUDTask final : public IExecutable {
public:
    void execute(absolute_time_t timestamp) override;
};
