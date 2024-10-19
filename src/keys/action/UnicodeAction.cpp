#include "UnicodeAction.h"

extern HIDKeyboard hidKeyboard;

uint8_t hexKeyCodes[] = {
        HID_KEY_0,
        HID_KEY_1,
        HID_KEY_2,
        HID_KEY_3,
        HID_KEY_4,
        HID_KEY_5,
        HID_KEY_6,
        HID_KEY_7,
        HID_KEY_8,
        HID_KEY_9,
        HID_KEY_A,
        HID_KEY_B,
        HID_KEY_C,
        HID_KEY_D,
        HID_KEY_E,
        HID_KEY_F,
};

hid_keyboard_report_t initReport = {
        KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT,
        0,
        {HID_KEY_U, 0, 0, 0, 0, 0}
};

hid_keyboard_report_t finishReport = {
        0,
        0,
        {HID_KEY_ENTER, 0, 0, 0, 0, 0}
};

hid_keyboard_report_t emptyReport = {
        0,
        0,
        {0, 0, 0, 0, 0, 0}
};

[[maybe_unused]] UnicodeAction::UnicodeAction(uint32_t unicode) : reports() {
    for (int i = 0; i < UnicodeLength; ++i) {
        reports[i] = {
                0,
                0,
                {hexKeyCodes[(unicode >> ((UnicodeLength - i - 1) * 4)) & 0xF], 0, 0, 0, 0, 0}
        };
    }
}

void UnicodeAction::execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) {
    hidKeyboard.addCustomReport(&emptyReport);
    hidKeyboard.addCustomReport(&initReport);
    for (hid_keyboard_report_t &report: reports) {
        hidKeyboard.addCustomReport(&report);
        hidKeyboard.addCustomReport(&emptyReport);
    }
    hidKeyboard.addCustomReport(&finishReport);
    hidKeyboard.addCustomReport(&emptyReport);
}
