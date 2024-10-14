#pragma once

#include "InterDeviceCommunicator.h"
#include "commands.h"

using CommandCreator = std::function<std::unique_ptr<Command>(InterDeviceCommunicator &)>;

template<typename Cmd>
concept DerivedFromCommand = std::is_base_of_v<Command, Cmd>;

class CommandController : public IExecutable {
private:
    InterDeviceCommunicator &communicator;
    std::unordered_map<uint16_t, CommandCreator> commandMap;

    template<DerivedFromCommand Cmd>
    void registerCommand();

public:
    CommandController(InterDeviceCommunicator &communicator, bool isPrimary);

    template<DerivedFromCommand Cmd, typename ... Args>
    void send(Args &&... args) {
        Cmd cmd(std::forward<Args>(args)...);
        cmd.send(communicator);
    }

    void execute(absolute_time_t timestamp) override;
};
