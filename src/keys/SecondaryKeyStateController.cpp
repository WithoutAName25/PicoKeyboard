#include "SecondaryKeyStateController.h"

#include "communication.h"

extern CommandController commandController;

SecondaryKeyStateController::SecondaryKeyStateController(uint8_t numKeys) : KeyStateController(numKeys) {}

void SecondaryKeyStateController::updateKeyState(uint8_t keyId, bool isPressed, absolute_time_t timestamp) {
    KeyStateController::updateKeyState(keyId, isPressed, timestamp);
    commandController.send<KeyCommand>(isPressed, keyId);
}
