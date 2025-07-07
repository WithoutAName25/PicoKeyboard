#pragma once

#include <util.h>

#include "KeyStateController.h"

class IKeyListener : public IExecutable {
protected:
    KeyStateController &controller;
    uint8_t numKeys;

public:
    IKeyListener(KeyStateController &controller, uint8_t numKeys);

    virtual void setupPins() = 0;
};
