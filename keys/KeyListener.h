#ifndef PICOKEYBOARD_KEYLISTENER_H
#define PICOKEYBOARD_KEYLISTENER_H

#include <memory>
#include "pico/stdlib.h"
#include "KeyStateController.h"
#include "../util/IExecutable.h"
#include "../util/Scheduler.h"

struct HWKeyConfig {
    uint8_t pin;
    uint8_t id;
};

class KeyListener : public IExecutable {
private:
    KeyStateController &controller;
    HWKeyConfig *keys;
    uint8_t numKeys;

public:
    KeyListener(Scheduler &scheduler, KeyStateController &controller, HWKeyConfig *keys, uint8_t numKeys);

    void setupPins();

    void execute(absolute_time_t timestamp) override;
};

#endif //PICOKEYBOARD_KEYLISTENER_H
