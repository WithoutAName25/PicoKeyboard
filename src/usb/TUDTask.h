#pragma once

#include "../util/IExecutable.h"

class TUDTask : public IExecutable {
public:
    void execute(absolute_time_t timestamp) override;
};
