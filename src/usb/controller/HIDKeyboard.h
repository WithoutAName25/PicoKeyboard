#pragma once

#include "HIDController.h"
#include "tusb.h"
#include "../usb_descriptors.h"

enum class Modifier {
    LEFT_CTRL,
    LEFT_SHIFT,
    LEFT_ALT,
    LEFT_GUI,
    RIGHT_CTRL,
    RIGHT_SHIFT,
    RIGHT_ALT,
    RIGHT_GUI
};

struct KeyPressState {
    uint8_t keycode;
    uint8_t count;
};

class HIDKeyboard : public HIDController<hid_keyboard_report_t, REPORT_ID_KEYBOARD> {
private:
    uint8_t modifierPressCounts[8] = {0};
    std::vector<KeyPressState> keyPressStates;
    bool lastReportEmpty = false;
public:
    HIDKeyboard() = default;

    void pressModifier(Modifier modifier);

    void releaseModifier(Modifier modifier);

    void pressKey(uint8_t keycode);

    void releaseKey(uint8_t keycode);

    bool hasReport() override;

    hid_keyboard_report_t getReport() override;
};
