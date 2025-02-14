#pragma once
#include "Command.h"

#include <rgb.h>

class RGBEffectCommand final : public Command {
    std::shared_ptr<IRGBEffect> effect;

    void execute(absolute_time_t timestamp) override;

public:
    static constexpr uint16_t ID = static_cast<uint16_t>(PrimaryToSecondaryCommandIDs::RGB_EFFECT);

    explicit RGBEffectCommand(const std::shared_ptr<IRGBEffect>& effect);

    explicit RGBEffectCommand(InterDeviceCommunicator& communicator);

    void send(InterDeviceCommunicator& communicator) override;
};
