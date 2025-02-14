#include "KeyCommand.h"

#include "keys.h"

extern KeyStateController keyStateController;

void KeyCommand::execute(const absolute_time_t timestamp) {
    keyStateController.updateKeyState(keyId, isPressed, timestamp, false);
}

KeyCommand::KeyCommand(const bool isPressed, const uint8_t keyId)
    : isPressed(isPressed), keyId(keyId) {}

KeyCommand::KeyCommand(InterDeviceCommunicator& communicator) {
    const uint8_t data = communicator.receive();
    isPressed = data & 0x40;
    keyId = data & 0x3F;
}

void KeyCommand::send(InterDeviceCommunicator& communicator) {
    communicator.send(ID << 7 | (isPressed ? 0x40 : 0) | keyId & 0x3F);
}
