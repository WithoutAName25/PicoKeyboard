#pragma once

#include "LCDLibrary.h"
#include "rotary_encoder.h"

class PongGame {
private:
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

    void drawState(State state, uint16_t color);

    void updatePosition(RotaryEncoder *enc, int32_t &lastRotation, int &pos);

    void updateBall();

public:
    PongGame(RotaryEncoder *left, RotaryEncoder *right, LCDGraphics *display);

    inline void init() {
        display->clear(0);
        reset();
    }

    void update();

    void draw();

    void reset();
};
