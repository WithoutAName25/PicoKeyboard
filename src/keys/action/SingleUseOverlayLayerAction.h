#pragma once

#include "../KeyLayer.h"
#include "../KeyActionController.h"

class [[maybe_unused]] SingleUseOverlayLayerAction : public IKeyAction, private IKeyStateListener {
private:
    KeyLayer &layer;
    OverlayLayerReference reference;
    KeyStateListenerReference listenerReference;

    void onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) override;

public:
    [[maybe_unused]] explicit SingleUseOverlayLayerAction(KeyLayer &layer);

    void execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) override;
};
