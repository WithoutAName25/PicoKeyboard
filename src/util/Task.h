#pragma once

#include "pico/stdlib.h"
#include "IExecutable.h"
#include <functional>

struct Task {
    IExecutable *executable;

    absolute_time_t nextExecutionTime;

    uint64_t period; // 0 means no periodic execution

    bool cancelled;

    Task(IExecutable *executable, absolute_time_t executionTime)
            : executable(executable), nextExecutionTime(executionTime), period(0), cancelled(false) {}

    Task(IExecutable *executable, absolute_time_t executionTime, uint64_t period)
            : executable(executable), nextExecutionTime(executionTime), period(period), cancelled(false) {}

    [[nodiscard]] bool isPeriodic() const {
        return period > 0;
    }

    void cancel() {
        cancelled = true;
    }
};

struct CompareTask {
    bool operator()(Task *t1, Task *t2) const {
        return t1->nextExecutionTime > t2->nextExecutionTime;
    }
};
