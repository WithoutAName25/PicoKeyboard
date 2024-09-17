#include "Scheduler.h"

Scheduler::Scheduler() : queue(std::make_unique<PriorityQueue>()) {}

void Scheduler::addTask(IExecutable *executable, absolute_time_t executionTime) {
    queue->push(Task(executable, executionTime));
}

void Scheduler::addPeriodicTask(IExecutable *executable, absolute_time_t firstExecutionTime,
                                uint64_t period) {
    queue->push(Task(executable, firstExecutionTime, period));
}

void Scheduler::run() {
    while (!queue->empty()) {
        absolute_time_t executionTime = queue->top().nextExecutionTime;
        sleep_until(executionTime);
        Task task = queue->top();
        queue->pop();
        task.executable->execute(executionTime);
        if (task.period > 0) {
            task.nextExecutionTime += task.period;
            queue->push(task);
        }
    }
}
