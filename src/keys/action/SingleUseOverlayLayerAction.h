#pragma once

#include "../KeyLayer.h"
#include "../KeyActionController.h"

class [[maybe_unused]] SingleUseOverlayLayerAction final : public IKeyAction, IKeyStateListener {
    KeyLayer& layer;
    OverlayLayerReference reference;
    KeyStateListenerReference listenerReference;
    bool isActive = false;

    void onKeyStateChange(uint8_t keyId, const KeyState& state, absolute_time_t timestamp) override;

public:
    [[maybe_unused]] explicit SingleUseOverlayLayerAction(KeyLayer& layer);

    void execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) override;
};
