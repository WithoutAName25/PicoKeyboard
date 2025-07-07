#pragma once

#include <functional>

#include "IExecutable.h"

class LambdaExecutable final : public IExecutable {
    std::function<void(absolute_time_t timestamp)> block;

public:
    explicit LambdaExecutable(const std::function<void(absolute_time_t timestamp)> &block);

    void execute(absolute_time_t timestamp) override;
};
