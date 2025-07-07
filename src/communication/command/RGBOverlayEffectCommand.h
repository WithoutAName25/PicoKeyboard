#pragma once
#include "Command.h"

#include <rgb.h>

class RGBOverlayEffectCommand final : public Command {
    std::shared_ptr<IRGBEffect> effect;
    uint16_t indexToRemove;

    void execute(absolute_time_t timestamp) override;

public:
    static constexpr uint16_t ID = static_cast<uint16_t>(PrimaryToSecondaryCommandIDs::RGB_OVERLAY_EFFECT);

    explicit RGBOverlayEffectCommand(const std::shared_ptr<IRGBEffect> &effect);

    explicit RGBOverlayEffectCommand(uint16_t indexToRemove);

    explicit RGBOverlayEffectCommand(InterDeviceCommunicator &communicator);

    void send(InterDeviceCommunicator &communicator) override;
};
