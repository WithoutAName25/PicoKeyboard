#pragma once

#include "HIDController.h"
#include "tusb.h"
#include "../usb_descriptors.h"

class HIDKeyboard : public HIDController<hid_keyboard_report_t, REPORT_ID_KEYBOARD> {
public:
    bool hasReport() override;

    hid_keyboard_report_t getReport() override;
};
