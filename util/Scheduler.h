#ifndef PICOKEYBOARD_SCHEDULER_H
#define PICOKEYBOARD_SCHEDULER_H

#include <functional>
#include <queue>
#include <memory>
#include "pico/stdlib.h"
#include "Task.h"

class Scheduler {
private:
    using PriorityQueue = std::priority_queue<Task, std::vector<Task>, CompareTask>;

    std::unique_ptr<PriorityQueue> queue;
public:
    Scheduler();

    void addTask(std::function<void(uint64_t)> *task, uint64_t executionTime);

    void addPeriodicTask(std::function<void(uint64_t)> *task, uint64_t firstExecutionTime, uint64_t period);

    void tick(uint64_t timestamp);
};


#endif //PICOKEYBOARD_SCHEDULER_H
