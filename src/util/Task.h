#pragma once

#include "IExecutable.h"

struct Task {
    IExecutable *executable;

    absolute_time_t nextExecutionTime;

    uint64_t period; // 0 means no periodic execution

    bool cancelled;

    Task(IExecutable *executable, const absolute_time_t executionTime)
        : executable(executable), nextExecutionTime(executionTime), period(0), cancelled(false) {}

    Task(IExecutable *executable, const absolute_time_t executionTime, const uint64_t period)
        : executable(executable), nextExecutionTime(executionTime), period(period), cancelled(false) {}

    [[nodiscard]] bool isPeriodic() const { return period > 0; }

    void cancel() { cancelled = true; }
};

struct CompareTask {
    bool operator()(const Task *t1, const Task *t2) const { return t1->nextExecutionTime > t2->nextExecutionTime; }
};
