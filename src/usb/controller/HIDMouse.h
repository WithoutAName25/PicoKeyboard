#pragma once

#include "HIDController.h"
#include "tusb.h"
#include "../usb_descriptors.h"

class HIDMouse final : public HIDController<hid_mouse_report_t, REPORT_ID_MOUSE> {
public:
    bool hasReport() override;

    hid_mouse_report_t getReport() override;
};
