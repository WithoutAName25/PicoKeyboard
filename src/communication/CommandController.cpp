#include "CommandController.h"
#include "command/KeyCommand.h"

template<DerivedFromCommand Cmd>
void CommandController::registerCommand() {
    commandMap[Cmd::ID] = [](InterDeviceCommunicator &comm) {
        return std::make_unique<Cmd>(comm);
    };
}

CommandController::CommandController(InterDeviceCommunicator &communicator, bool isPrimary) : communicator(
        communicator), commandMap() {
    if (isPrimary) {
        registerCommand<KeyCommand>();
    } else {
    }
}

void CommandController::execute(absolute_time_t timestamp) {
    while (communicator.hasData()) {
        bool successful = false;
        uint8_t firstByte = communicator.peek();
        for (uint8_t numBits = 0; numBits <= 8; ++numBits) {
            uint16_t id = firstByte >> (8 - numBits) | 1 << numBits;
            auto it = commandMap.find(id);
            if (it != commandMap.end()) {
                std::unique_ptr<Command> command = it->second(communicator);
                if (command) {
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
