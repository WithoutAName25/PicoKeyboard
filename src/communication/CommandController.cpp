#include "CommandController.h"
#include "command/KeyCommand.h"

CommandController::CommandController(InterDeviceCommunicator &communicator, const bool isPrimary) : communicator(
        communicator) {
    if (isPrimary) {
        registerCommand<KeyCommand>();
    } else {
    }
}

void CommandController::execute(const absolute_time_t timestamp) {
    while (communicator.hasData()) {
        bool successful = false;
        const uint8_t firstByte = communicator.peek();
        for (uint8_t numBits = 0; numBits <= 8; ++numBits) {
            const uint16_t id = firstByte >> (8 - numBits) | 1 << numBits;
            if (auto it = commandMap.find(id); it != commandMap.end()) {
                if (const std::unique_ptr<Command> command = it->second(communicator)) {
                    command->execute(timestamp);
                    successful = true;
                    break;
                }
            }
        }
        if (!successful) {
            communicator.receive();
        }
    }
}
