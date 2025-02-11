#pragma once

#include "../KeyLayer.h"

class [[maybe_unused]] BaseLayerAction final : public IKeyAction {
    KeyLayer &layer;

public:
    [[maybe_unused]] explicit BaseLayerAction(KeyLayer &layer);

    void execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) override;
};
