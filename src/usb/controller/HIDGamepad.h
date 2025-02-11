#pragma once

#include "HIDController.h"
#include "tusb.h"
#include "../usb_descriptors.h"

class HIDGamepad final : public HIDController<hid_gamepad_report_t, REPORT_ID_GAMEPAD> {
public:
    bool hasReport() override;

    hid_gamepad_report_t getReport() override;
};
