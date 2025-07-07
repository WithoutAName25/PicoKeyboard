#include "LambdaExecutable.h"
LambdaExecutable::LambdaExecutable(const std::function<void(absolute_time_t timestamp)> &block) : block(block) {}

void LambdaExecutable::execute(const absolute_time_t timestamp) { block(timestamp); }
