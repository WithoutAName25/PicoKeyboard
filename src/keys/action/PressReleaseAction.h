#pragma once

#include "IKeyAction.h"
#include "IPressReleaseAction.h"

class PressReleaseAction
        : public IKeyAction, public IPressReleaseAction, private IKeyStateListener {
private:
    uint8_t activatedBy = 0;
    KeyStateListenerReference listenerReference;
    absolute_time_t activationTimestamp = 0;

    void onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) override;

public:
    void execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) override;

};
