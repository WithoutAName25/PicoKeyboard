#pragma once

#include <functional>
#include <unordered_map>

#include "InterDeviceCommunicator.h"
#include "commands.h"

using CommandCreator = std::function<std::unique_ptr<Command>(InterDeviceCommunicator&)>;

template <typename Cmd>
concept DerivedFromCommand = std::is_base_of_v<Command, Cmd>;

class CommandController final : public IExecutable {
    InterDeviceCommunicator& communicator;
    std::unordered_map<uint16_t, CommandCreator> commandMap;

    template <DerivedFromCommand Cmd>
    void listenFor() {
        commandMap[Cmd::ID] = [](InterDeviceCommunicator& comm) {
            return std::make_unique<Cmd>(comm);
        };
    }

public:
    CommandController(InterDeviceCommunicator& communicator, bool isPrimary);

    template <DerivedFromCommand Cmd, typename... Args>
    void send(Args&&... args) {
        Cmd cmd(std::forward<Args>(args)...);
        cmd.send(communicator);
    }

    void execute(absolute_time_t timestamp) override;
};
