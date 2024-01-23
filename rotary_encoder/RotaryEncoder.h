
#ifndef PICOKEYBOARD_ROTARYENCODER_H
#define PICOKEYBOARD_ROTARYENCODER_H

#include "pico/stdlib.h"

class RotaryEncoder {
private:
    uint pinA;
    uint pinB;
    int32_t rotation = 0;
    bool lastChange = false; // false = A, true = B
    bool state = false; // state of unchanged pin

public:

    RotaryEncoder(uint pinA, uint pinB) : pinA(pinA), pinB(pinB) {}

    void init() const;

    void callback(uint gpio);

    [[nodiscard]] int32_t getRotation() const {
        return rotation;
    }

    void resetRotation() {
        rotation = 0;
    }
};


#endif //PICOKEYBOARD_ROTARYENCODER_H
