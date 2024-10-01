#pragma once

#include "pico/stdlib.h"
#include "util.h"

template<typename ReportType, uint8_t ReportID>
class HIDController : public IExecutable {
public:
    virtual bool hasReport() = 0;

    virtual ReportType getReport() = 0;

    void execute(absolute_time_t timestamp) override;
};
