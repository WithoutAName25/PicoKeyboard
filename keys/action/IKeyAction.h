
#ifndef PICOKEYBOARD_IKEYACTION_H
#define PICOKEYBOARD_IKEYACTION_H

#include "pico/stdlib.h"
#include "../KeyStateController.h"

class IKeyAction {
public:
    virtual ~IKeyAction() = default;

    virtual void execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) = 0;
};


#endif //PICOKEYBOARD_IKEYACTION_H
