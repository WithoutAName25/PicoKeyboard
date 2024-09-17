#ifndef PICOKEYBOARD_KEYLISTENER_H
#define PICOKEYBOARD_KEYLISTENER_H

#include <memory>
#include "pico/stdlib.h"
#include "KeyStateManager.h"
#include "../util/IExecutable.h"

struct HWKeyConfig {
    uint8_t pin;
    uint8_t id;
};

class KeyListener : public IExecutable {
private:
    KeyStateManager *manager;
    std::unique_ptr<HWKeyConfig[]> keys;
    uint8_t numKeys;

public:
    KeyListener(KeyStateManager *manager, std::unique_ptr<HWKeyConfig[]> keys, uint8_t numKeys);

    void setupPins();

    void execute(absolute_time_t timestamp) override;
};

#endif //PICOKEYBOARD_KEYLISTENER_H
