#pragma once

#include "HIDController.h"
#include "tusb.h"
#include "../usb_descriptors.h"
#include <list>

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
    absolute_time_t activation;
    bool reported;

    KeyPressState(const uint8_t keycode, const uint8_t count, const absolute_time_t activation)
            : keycode(keycode), count(count), activation(activation), reported(false) {}
};

using KeyBlockingReference = std::list<absolute_time_t>::iterator;

class HIDKeyboard final : public HIDController<hid_keyboard_report_t, REPORT_ID_KEYBOARD> {
    uint8_t modifierPressCounts[8] = {};
    std::vector<KeyPressState> keyPressStates;
    std::list<absolute_time_t> blockingTimes;
    bool lastReportEmpty = false;

public:
    HIDKeyboard() = default;

    void pressModifier(Modifier modifier);

    void releaseModifier(Modifier modifier);

    void pressKey(uint8_t keycode, absolute_time_t timestamp);

    void releaseKey(uint8_t keycode, absolute_time_t timestamp);

    KeyBlockingReference blockNewKeys(absolute_time_t timestamp);

    void unblockNewKeys(KeyBlockingReference reference);

    bool hasReport() override;

    hid_keyboard_report_t getReport() override;
};
