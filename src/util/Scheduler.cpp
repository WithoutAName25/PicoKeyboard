#include "Scheduler.h"

Scheduler::Scheduler() : queue(std::make_unique<PriorityQueue>()) {}

Task *Scheduler::addTask(IExecutable *executable, absolute_time_t executionTime) {
    Task *task = new Task(executable, executionTime);
    queue->push(task);
    return task;
}

Task *Scheduler::addPeriodicTask(IExecutable *executable, absolute_time_t firstExecutionTime,
                                 uint64_t period) {
    Task *task = new Task(executable, firstExecutionTime, period);
    queue->push(task);
    return task;
}

void Scheduler::run() {
    while (!queue->empty()) {
        Task *task = queue->top();
        absolute_time_t executionTime = task->nextExecutionTime;
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
