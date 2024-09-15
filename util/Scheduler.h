#ifndef PICOKEYBOARD_SCHEDULER_H
#define PICOKEYBOARD_SCHEDULER_H

#include <functional>
#include <queue>
#include <memory>
#include "pico/stdlib.h"
#include "Task.h"

class Scheduler {
private:
    std::unique_ptr<std::priority_queue<Task, std::vector<Task>, CompareTask>> queue;
public:
    void addTask(std::function<void()> *task, uint64_t executionTime);

    void addPeriodicTask(std::function<void()> *task, uint64_t firstExecutionTime, uint64_t period);

    void tick(uint64_t timestamp);
};


#endif //PICOKEYBOARD_SCHEDULER_H
