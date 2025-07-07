#include "UnicodeAction.h"

#define UNICODE_INPUT_INIT_DELAY 50000

extern HIDKeyboard hidKeyboard;
extern Scheduler scheduler;

uint8_t hexKeyCodes[] = {
    HID_KEY_0, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7,
    HID_KEY_8, HID_KEY_9, HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F,
};

hid_keyboard_report_t initReport = {
    KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT, 0, {HID_KEY_U, 0, 0, 0, 0, 0}};

hid_keyboard_report_t finishReport = {0, 0, {HID_KEY_ENTER, 0, 0, 0, 0, 0}};

hid_keyboard_report_t emptyReport = {0, 0, {0, 0, 0, 0, 0, 0}};

void UnicodeAction::execute(absolute_time_t timestamp) {
    bool leadingZeros = true;
    for (int i = 0; i < UnicodeLength; ++i) {
        if (leadingZeros && reports[i].keycode[0] == HID_KEY_0)
            continue;

        leadingZeros = false;
        hidKeyboard.addCustomReport(&reports[i]);
        if (i != UnicodeLength - 1 && reports[i].keycode[0] == reports[i + 1].keycode[0]) {
            hidKeyboard.addCustomReport(&emptyReport);
        }
    }
    hidKeyboard.addCustomReport(&finishReport);
    hidKeyboard.addCustomReport(&emptyReport);
}

[[maybe_unused]] UnicodeAction::UnicodeAction(const uint32_t unicode) : reports() {
    for (int i = 0; i < UnicodeLength; ++i) {
        reports[i] = {0, 0, {hexKeyCodes[(unicode >> ((UnicodeLength - i - 1) * 4)) & 0xF], 0, 0, 0, 0, 0}};
    }
}

void UnicodeAction::execute(uint8_t keyId, const KeyState *state, const absolute_time_t timestamp) {
    hidKeyboard.addCustomReport(&emptyReport);
    hidKeyboard.addCustomReport(&initReport);
    hidKeyboard.addCustomReport(&emptyReport);
    scheduler.addTask(this, timestamp + UNICODE_INPUT_INIT_DELAY);
}
