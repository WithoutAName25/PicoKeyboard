#ifndef PICOKEYBOARD_KEYLISTENER_H
#define PICOKEYBOARD_KEYLISTENER_H

#include <memory>
#include "pico/stdlib.h"
#include "KeyStateManager.h"

struct HWKeyConfig {
    uint8_t pin;
    uint8_t id;
};

class KeyListener {
private:
    KeyStateManager *manager;
    std::unique_ptr<HWKeyConfig[]> keys;
    uint8_t numKeys;

public:
    KeyListener(KeyStateManager *manager, std::unique_ptr<HWKeyConfig[]> keys, uint8_t numKeys);

    void setupPins();

    void tick(absolute_time_t timestamp);
};

#endif //PICOKEYBOARD_KEYLISTENER_H
