#include "HIDController.h"
#include "tusb.h"
#include "HIDCommunication.h"

template
class HIDController<hid_keyboard_report_t, 1>;

template
class HIDController<hid_mouse_report_t, 2>;

template
class HIDController<unsigned short, 3>;

template
class HIDController<hid_gamepad_report_t, 4>;

template
class HIDController<hid_communication_report_t, 5>;

template<typename ReportType, uint8_t ReportID>
void HIDController<ReportType, ReportID>::execute(absolute_time_t timestamp) {
    if (!tud_hid_ready()) return;
    if (!hasReport()) return;
    if (tud_suspended()) {
        tud_remote_wakeup();
    }

    ReportType report = getReport();
    tud_hid_report(ReportID, &report, sizeof(ReportType));
}