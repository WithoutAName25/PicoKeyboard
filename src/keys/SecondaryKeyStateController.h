#pragma once

#include "KeyStateController.h"

class SecondaryKeyStateController : public KeyStateController {
public:
    explicit SecondaryKeyStateController(uint8_t numKeys);

    void updateKeyState(uint8_t keyId, bool isPressed, absolute_time_t timestamp) override;
};
