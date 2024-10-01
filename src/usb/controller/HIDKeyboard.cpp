#include "HIDKeyboard.h"

bool HIDKeyboard::hasReport() {
    return false;
}

hid_keyboard_report_t HIDKeyboard::getReport() {
    return {};
}
