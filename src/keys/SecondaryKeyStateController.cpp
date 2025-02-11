#include "SecondaryKeyStateController.h"

#include "communication.h"

extern CommandController commandController;

SecondaryKeyStateController::SecondaryKeyStateController(const uint8_t numKeys) : KeyStateController(numKeys) {}

void SecondaryKeyStateController::updateKeyState(uint8_t keyId, bool isPressed, const absolute_time_t timestamp) {
    if (getKeyState(keyId).isPressed != isPressed) {
        commandController.send<KeyCommand>(isPressed, keyId);
    }
    KeyStateController::updateKeyState(keyId, isPressed, timestamp);
}
