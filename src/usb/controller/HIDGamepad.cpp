#include "HIDGamepad.h"

bool HIDGamepad::hasReport() { return false; }

hid_gamepad_report_t HIDGamepad::getReport() { return {}; }
