#pragma once

#include <memory>
#include "pico/stdlib.h"
#include "KeyStateController.h"
#include "../util/IExecutable.h"

struct HWKeyConfig {
    uint8_t pin;
    uint8_t id;

    HWKeyConfig(const uint8_t pin, const uint8_t id) : pin(pin), id(id) {}
};

class KeyListener final : public IExecutable {
    KeyStateController& controller;
    HWKeyConfig* keys;
    uint8_t numKeys;

public:
    KeyListener(KeyStateController& controller, HWKeyConfig* keys, uint8_t numKeys);

    void setupPins() const;

    void execute(absolute_time_t timestamp) override;
};
