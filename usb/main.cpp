#include "main.h"
#include "tusb.h"
#include "usb_descriptors.h"

// Invoked when device is mounted
void tud_mount_cb(void) {
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void sendKeyboardReport(KeyLayerManager *keys) {
    // skip if hid is not ready yet
    if (!tud_hid_ready()) return;

    // use to avoid send multiple consecutive zero report for keyboard
    static bool has_keyboard_key = false;

    const std::vector<uint8_t> &pressedKeys = keys->getPressedKeys();
    if (!pressedKeys.empty()) {
        uint8_t keycode[6] = {0};
        for (int i = 0; i < 6 && i < pressedKeys.size(); ++i) {
            keycode[i] = pressedKeys[i];
        }

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
    } else {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
    }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(KeyLayerManager *keys) {
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if ((time_us_64() >> 10) - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    size_t numPressedKeys = keys->getPressedKeys().size();

    // Remote wakeup
    if (tud_suspended() && numPressedKeys) {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    } else {
        // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
        sendKeyboardReport(keys);
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len) {
    (void) instance;
    (void) report;
    (void) len;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
    (void) instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT) {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD) {
            // bufsize should be (at least) 1
            if (bufsize < 1) return;

            uint8_t const kbd_leds = buffer[0];
            bool caps_lock = kbd_leds & KEYBOARD_LED_CAPSLOCK;
            (void) caps_lock;
            // TODO show caps_lock and other leds
        }
    }
}