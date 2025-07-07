#pragma once

#include "Task.h"
#include "pico/stdlib.h"
#include <memory>
#include <queue>

class Scheduler {
    using PriorityQueue = std::priority_queue<Task *, std::vector<Task *>, CompareTask>;

    std::unique_ptr<PriorityQueue> queue;

public:
    Scheduler();

    Task *addTask(IExecutable *executable, absolute_time_t executionTime) const;

    Task *addPeriodicTask(IExecutable *executable, absolute_time_t firstExecutionTime, uint64_t period) const;

    void run() const;
};
