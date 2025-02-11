#include "KeyCommand.h"

#include "keys.h"

extern KeyStateController keyStateController;

KeyCommand::KeyCommand(const bool isPressed, const uint8_t keyId)
    : isPressed(isPressed), keyId(keyId) {}

KeyCommand::KeyCommand(InterDeviceCommunicator& communicator)
    : isPressed(communicator.peek() >> 7), keyId(communicator.receive() & 0x7F) {}

void KeyCommand::send(InterDeviceCommunicator& communicator) {
    communicator.send((isPressed ? 1 : 0) << 7 | keyId & 0x7F);
}

void KeyCommand::execute(const absolute_time_t timestamp) {
    keyStateController.updateKeyState(keyId, isPressed, timestamp);
}
