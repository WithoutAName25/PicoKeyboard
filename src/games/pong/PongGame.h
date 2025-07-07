#pragma once

#include "LCDLibrary.h"
#include "rotary_encoder.h"

class PongGame {
    struct Vector {
        int x{};
        int y{};
    };

    struct State {
        int posLeft{};
        int posRight{};
        Vector ballPos;
        Vector ballSpeed;
    };

    RotaryEncoder *left;
    RotaryEncoder *right;
    LCDGraphics *display;
    int32_t rotationLeft{};
    int32_t rotationRight{};
    State currentState;
    State oldState;

    void drawState(const State &state, uint16_t color) const;

    void updatePosition(const RotaryEncoder *enc, int32_t &lastRotation, int &pos) const;

    void updateBall();

public:
    PongGame(RotaryEncoder *left, RotaryEncoder *right, LCDGraphics *display);

    void init() {
        display->clear(0);
        reset();
    }

    void update();

    void draw() const;

    void reset();
};
