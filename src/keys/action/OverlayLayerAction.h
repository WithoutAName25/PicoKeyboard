#pragma once

#include "../KeyLayer.h"
#include "PressReleaseAction.h"
#include "../KeyActionController.h"

class [[maybe_unused]] OverlayLayerAction : public PressReleaseAction {
private:
    KeyLayer &layer;
    OverlayLayerReference reference;

public:
    [[maybe_unused]] explicit OverlayLayerAction(KeyLayer &layer);

    void press(absolute_time_t timestamp) override;

    void release(absolute_time_t timestamp) override;
};
