#include "PongGame.h"

#define PADDLE_SPEED 2
#define PADDLE_SIZE 20
#define PADDLE_HALF_SIZE (PADDLE_SIZE / 2)
#define PADDLE_WIDTH 5
#define PADDLE_OFFSET 10
#define PADDLE_MIN_POS PADDLE_HALF_SIZE
#define PADDLE_MAX_POS (display->getHeight() - PADDLE_HALF_SIZE - 1)
#define BALL_RADIUS 2

void PongGame::drawState(PongGame::State state, uint16_t color) {
    display->fillRect(PADDLE_OFFSET, state.posLeft - PADDLE_HALF_SIZE, PADDLE_OFFSET + PADDLE_WIDTH - 1,
                      state.posLeft + PADDLE_HALF_SIZE, color);
    display->fillRect(display->getWidth() - PADDLE_OFFSET - PADDLE_WIDTH, state.posRight - PADDLE_HALF_SIZE,
                      display->getWidth() - PADDLE_OFFSET - 1,
                      state.posRight + PADDLE_HALF_SIZE, color);
    display->fillRect(state.ballPos.x - BALL_RADIUS, state.ballPos.y - BALL_RADIUS, state.ballPos.x + BALL_RADIUS,
                      state.ballPos.y + BALL_RADIUS, color);
}

void PongGame::updatePosition(RotaryEncoder *enc, int32_t &lastRotation, int &pos) {
    int32_t rotation = enc->getRotation();
    int32_t rotationDiff = rotation - lastRotation;
    pos += PADDLE_SPEED * rotationDiff;
    lastRotation = rotation;
    if (pos < PADDLE_MIN_POS) pos = PADDLE_MIN_POS;
    else if (pos > PADDLE_MAX_POS) pos = PADDLE_MAX_POS;
}

void PongGame::updateBall() {
    currentState.ballPos.x += currentState.ballSpeed.x;
    currentState.ballPos.y += currentState.ballSpeed.y;

    // reflect top and bottom
    if (currentState.ballPos.y <= BALL_RADIUS || currentState.ballPos.y >= display->getHeight() - BALL_RADIUS - 1) {
        currentState.ballSpeed.y = -currentState.ballSpeed.y;
    }

    // reflect on paddles
    if (oldState.ballPos.x - BALL_RADIUS > PADDLE_OFFSET + PADDLE_WIDTH &&
        currentState.ballPos.x - BALL_RADIUS <= PADDLE_OFFSET + PADDLE_WIDTH &&
        currentState.ballPos.y + BALL_RADIUS >= currentState.posLeft - PADDLE_HALF_SIZE &&
        currentState.ballPos.y - BALL_RADIUS <= currentState.posLeft + PADDLE_HALF_SIZE) {
        currentState.ballSpeed.x = -currentState.ballSpeed.x;
    } else if (oldState.ballPos.x + BALL_RADIUS < display->getWidth() - PADDLE_OFFSET - PADDLE_WIDTH - 1 &&
               currentState.ballPos.x + BALL_RADIUS >= display->getWidth() - PADDLE_OFFSET - PADDLE_WIDTH - 1 &&
               currentState.ballPos.y + BALL_RADIUS >= currentState.posRight - PADDLE_HALF_SIZE &&
               currentState.ballPos.y - BALL_RADIUS <= currentState.posRight + PADDLE_HALF_SIZE) {
        currentState.ballSpeed.x = -currentState.ballSpeed.x;
    }

    // count goals
    if (currentState.ballPos.x - BALL_RADIUS <= 0) {
        drawState(oldState, 0);
        reset();
    } else if (currentState.ballPos.x + BALL_RADIUS >= display->getWidth() - 1) {
        drawState(oldState, 0);
        reset();
    }
}

PongGame::PongGame(RotaryEncoder *left, RotaryEncoder *right, LCDGraphics *display)
        : left(left), right(right), display(display) {
}

void PongGame::update() {
    oldState = currentState;
    updatePosition(left, rotationLeft, currentState.posLeft);
    updatePosition(right, rotationRight, currentState.posRight);
    updateBall();
}


void PongGame::draw() {
    drawState(oldState, 0x0000);
    drawState(currentState, 0xFFFF);
}

void PongGame::reset() {
    rotationLeft = left->getRotation();
    rotationRight = right->getRotation();
    uint16_t halfHeight = display->getHeight() >> 1;
    uint16_t halfWidth = display->getWidth() >> 1;
    State state = {
            .posLeft = halfHeight,
            .posRight = halfHeight,
            .ballPos = {halfWidth, halfHeight},
            .ballSpeed = {1, 1}
    };
    currentState = state;
    oldState = state;
}
