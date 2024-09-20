#ifndef PICOKEYBOARD_SCHEDULER_H
#define PICOKEYBOARD_SCHEDULER_H

#include <queue>
#include <memory>
#include "Task.h"

class Scheduler {
private:
    using PriorityQueue = std::priority_queue<Task *, std::vector<Task *>, CompareTask>;

    std::unique_ptr<PriorityQueue> queue;
public:
    Scheduler();

    Task *addTask(IExecutable *executable, absolute_time_t executionTime);

    Task *addPeriodicTask(IExecutable *executable, absolute_time_t firstExecutionTime, uint64_t period);

    void run();
};


#endif //PICOKEYBOARD_SCHEDULER_H
