#include "Scheduler.h"

Scheduler::Scheduler() : queue(std::make_unique<PriorityQueue>()) {}

void Scheduler::addTask(std::function<void(absolute_time_t)> *task, absolute_time_t executionTime) {
    queue->push(Task(task, executionTime));
}

void Scheduler::addPeriodicTask(std::function<void(absolute_time_t)> *task, absolute_time_t firstExecutionTime,
                                uint64_t period) {
    queue->push(Task(task, firstExecutionTime, period));
}

void Scheduler::run() {
    while (!queue->empty()) {
        absolute_time_t executionTime = queue->top().nextExecutionTime;
        sleep_until(executionTime);
        Task task = queue->top();
        queue->pop();
        (*task.task)(executionTime);
        if (task.period > 0) {
            task.nextExecutionTime += task.period;
            queue->push(task);
        }
    }
}
