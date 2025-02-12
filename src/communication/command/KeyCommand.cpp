#include "KeyCommand.h"

#include "keys.h"

extern KeyStateController keyStateController;

KeyCommand::KeyCommand(const bool isPressed, const uint8_t keyId)
    : isPressed(isPressed), keyId(keyId) {}

KeyCommand::KeyCommand(InterDeviceCommunicator& communicator)
    : isPressed(communicator.peek() & 0x40), keyId(communicator.receive() & 0x3F) {}

void KeyCommand::send(InterDeviceCommunicator& communicator) {
    communicator.send(ID << 7 | (isPressed ? 0x40 : 0) | keyId & 0x3F);
}

void KeyCommand::execute(const absolute_time_t timestamp) {
    keyStateController.updateKeyState(keyId, isPressed, timestamp, false);
}
