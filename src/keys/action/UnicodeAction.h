#pragma once

#include "IKeyAction.h"
#include "usb.h"

#define UnicodeLength 6

class UnicodeAction : public IKeyAction {
    hid_keyboard_report_t reports[UnicodeLength + 2];
public:
    explicit UnicodeAction(uint32_t unicode);

    void execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) override;
};
