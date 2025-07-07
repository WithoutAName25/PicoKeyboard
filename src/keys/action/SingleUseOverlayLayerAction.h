#pragma once

#include "../KeyActionController.h"
#include "../KeyLayer.h"

class [[maybe_unused]] SingleUseOverlayLayerAction final : public IKeyAction, IKeyStateListener {
    KeyLayer &layer;
    OverlayLayerReference reference;
    KeyStateListenerReference listenerReference;
    bool isActive = false;
    absolute_time_t activationTimestamp = 0;

    void onKeyStateChange(uint8_t keyId, const KeyState &state, absolute_time_t timestamp) override;

public:
    [[maybe_unused]] explicit SingleUseOverlayLayerAction(KeyLayer &layer);

    void execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) override;
};
