#pragma once

#include <memory>

#include "IKeyListener.h"
#include "pico/stdlib.h"
#include "KeyStateController.h"
#include "../util/IExecutable.h"

struct HWKeyConfig {
    uint8_t pin;
    uint8_t id;

    HWKeyConfig(const uint8_t pin, const uint8_t id) : pin(pin), id(id) {}
};

class KeyListener final : public IKeyListener {
    HWKeyConfig* keys;

public:
    KeyListener(KeyStateController& controller, uint8_t numKeys, HWKeyConfig* keys);

    void setupPins() override;

    void execute(absolute_time_t timestamp) override;
};
