#pragma once

#include "pico/stdlib.h"
#include "util.h"
#include "tusb.h"

template<typename ReportType, uint8_t ReportID>
class HIDController : public IExecutable {
public:
    virtual bool hasReport() = 0;

    virtual ReportType getReport() = 0;

    void execute(absolute_time_t timestamp) override {
        if (!hasReport()) return;
        if (tud_suspended()) {
            tud_remote_wakeup();
            return;
        }
        if (!tud_hid_ready()) return;

        ReportType report = getReport();
        tud_hid_report(ReportID, &report, sizeof(ReportType));
    };
};
