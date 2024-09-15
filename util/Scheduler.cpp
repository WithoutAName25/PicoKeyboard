#include "Scheduler.h"

Scheduler::Scheduler() : queue(std::make_unique<PriorityQueue>()) {}

void Scheduler::addTask(std::function<void(uint64_t)> *task, uint64_t executionTime) {
    queue->push(Task(task, executionTime));
}

void Scheduler::addPeriodicTask(std::function<void(uint64_t)> *task, uint64_t firstExecutionTime, uint64_t period) {
    queue->push(Task(task, firstExecutionTime, period));
}

void Scheduler::tick(uint64_t timestamp) {
    while (!queue->empty() && queue->top().nextExecutionTime <= timestamp) {
        Task task = queue->top();
        queue->pop();
        (*task.task)(timestamp);
        if (task.period > 0) {
            task.nextExecutionTime += task.period;
            queue->push(task);
        }
    }
}
