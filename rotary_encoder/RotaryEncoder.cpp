
#include "RotaryEncoder.h"

void RotaryEncoder::init() const {
    gpio_init(pinA);
    gpio_init(pinB);
    gpio_pull_up(pinA);
    gpio_pull_up(pinB);
    gpio_set_irq_enabled(pinA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(pinB, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

void RotaryEncoder::callback(uint gpio) {
    if (lastChange) {
        if (gpio == pinA) {
            bool stateB = gpio_get(pinB);
            if (state == stateB) {
                if (rotation < 255)
                    rotation++;
            } else {
                if (rotation > 0)
                    rotation--;
            }
            lastChange = false;
            state = stateB;
        }
    } else {
        if (gpio == pinB) {
            bool stateA = gpio_get(pinA);
            if (state == stateA) {
                if (rotation > 0)
                    rotation--;
            } else {
                if (rotation < 255)
                    rotation++;
            }
            lastChange = true;
            state = stateA;
        }
    }
}
