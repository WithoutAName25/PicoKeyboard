#include "HIDMouse.h"

bool HIDMouse::hasReport() { return false; }

hid_mouse_report_t HIDMouse::getReport() { return {}; }
