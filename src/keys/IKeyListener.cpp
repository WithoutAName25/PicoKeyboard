#include "IKeyListener.h"

IKeyListener::IKeyListener(KeyStateController& controller, uint8_t numKeys)
    : controller(controller), numKeys(numKeys) {}
