#ifndef PICOKEYBOARD_TASK_H
#define PICOKEYBOARD_TASK_H

#include "pico/stdlib.h"
#include "IExecutable.h"
#include <functional>

struct Task {
    IExecutable *executable;

    absolute_time_t nextExecutionTime;

    uint64_t period; // 0 means no periodic execution

    Task(IExecutable *executable, absolute_time_t executionTime)
            : executable(executable), nextExecutionTime(executionTime), period(0) {}

    Task(IExecutable *executable, absolute_time_t executionTime, uint64_t period)
            : executable(executable), nextExecutionTime(executionTime), period(period) {}

    [[nodiscard]] bool isPeriodic() const {
        return period > 0;
    }
};

struct CompareTask {
    bool operator()(const Task &t1, const Task &t2) const {
        return t1.nextExecutionTime > t2.nextExecutionTime;
    }
};

#endif //PICOKEYBOARD_TASK_H
