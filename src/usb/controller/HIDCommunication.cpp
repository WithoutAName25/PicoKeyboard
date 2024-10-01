#include "HIDCommunication.h"

bool HIDCommunication::hasReport() {
    return false;
}

hid_communication_report_t HIDCommunication::getReport() {
    return {};
}
