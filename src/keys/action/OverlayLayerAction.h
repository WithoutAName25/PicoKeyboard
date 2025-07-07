#pragma once

#include "../KeyActionController.h"
#include "../KeyLayer.h"
#include "PressReleaseAction.h"

class [[maybe_unused]] OverlayLayerAction final : public PressReleaseAction {
    KeyLayer &layer;
    OverlayLayerReference reference;

public:
    [[maybe_unused]] explicit OverlayLayerAction(KeyLayer &layer);

    void press(absolute_time_t timestamp) override;

    void release(absolute_time_t timestamp) override;
};
