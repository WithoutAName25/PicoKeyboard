#pragma once

#include "HIDController.h"
#include "tusb.h"
#include "../usb_descriptors.h"

class HIDConsumerControl : public HIDController<uint16_t, REPORT_ID_CONSUMER_CONTROL> {
public:
    bool hasReport() override;

    uint16_t getReport() override;
};
