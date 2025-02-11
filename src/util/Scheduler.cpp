#include "Scheduler.h"

#include "pico/stdlib.h"

Scheduler::Scheduler() : queue(std::make_unique<PriorityQueue>()) {}

Task *Scheduler::addTask(IExecutable *executable, const absolute_time_t executionTime) const {
    const auto task = new Task(executable, executionTime);
    queue->push(task);
    return task;
}

Task *Scheduler::addPeriodicTask(IExecutable *executable, const absolute_time_t firstExecutionTime,
                                 const uint64_t period) const {
    const auto task = new Task(executable, firstExecutionTime, period);
    queue->push(task);
    return task;
}

void Scheduler::run() const {
    while (!queue->empty()) {
        Task *task = queue->top();
        const absolute_time_t executionTime = task->nextExecutionTime;
        sleep_until(executionTime);
        queue->pop();
        if (!task->cancelled) {
            task->executable->execute(executionTime);
        }
        if (task->isPeriodic() && !task->cancelled) {
            task->nextExecutionTime += task->period;
            queue->push(task);
        } else {
            delete task;
        }
    }
}
