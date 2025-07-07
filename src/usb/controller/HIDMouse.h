#pragma once

#include "../usb_descriptors.h"
#include "HIDController.h"
#include "tusb.h"

class HIDMouse final : public HIDController<hid_mouse_report_t, REPORT_ID_MOUSE> {
public:
    bool hasReport() override;

    hid_mouse_report_t getReport() override;
};
