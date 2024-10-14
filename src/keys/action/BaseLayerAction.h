#pragma once

#include "../KeyLayer.h"

class [[maybe_unused]] BaseLayerAction : public IKeyAction {
private:
    KeyLayer &layer;

public:
    [[maybe_unused]] explicit BaseLayerAction(KeyLayer &layer);

    void execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) override;
};
