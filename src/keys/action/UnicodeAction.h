#pragma once

#include "IKeyAction.h"
#include "usb.h"

#define UnicodeLength 6

class [[maybe_unused]] UnicodeAction final : public IKeyAction, IExecutable {
    hid_keyboard_report_t reports[UnicodeLength];

    void execute(absolute_time_t timestamp) override;

public:
    [[maybe_unused]] explicit UnicodeAction(uint32_t unicode);

    void execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) override;
};
