#pragma once

#include "../usb_descriptors.h"
#include "HIDController.h"

class HIDConsumerControl final : public HIDController<uint16_t, REPORT_ID_CONSUMER_CONTROL> {
    std::queue<uint16_t> queue;
    bool lastReportEmpty = false;

public:
    HIDConsumerControl();

    void sendCommand(uint16_t command);

    bool hasReport() override;

    uint16_t getReport() override;
};
