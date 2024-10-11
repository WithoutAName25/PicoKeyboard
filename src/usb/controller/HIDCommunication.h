#pragma once

#include "HIDController.h"
#include "tusb.h"
#include "../usb_descriptors.h"

struct hid_communication_report_t {
    uint8_t data[COMMUNICATION_REPORT_SIZE];
};

class HIDCommunication : public HIDController<hid_communication_report_t, REPORT_ID_COMMUNICATION> {
public:
    bool hasReport() override;

    hid_communication_report_t getReport() override;
};
