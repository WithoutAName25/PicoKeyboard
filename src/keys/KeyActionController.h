#pragma once

#include <memory>
#include "KeyLayer.h"

using OverlayLayerReference = std::list<KeyLayer *>::iterator;

class KeyActionController : IKeyStateListener {
private:
    KeyStateController &keyStateController;
    std::list<KeyLayer> layers;
    uint8_t numKeys;
    KeyLayer *currentBaseLayer;
    std::list<KeyLayer *> activeOverlayLayers;

    IKeyAction *getAction(uint8_t keyId);

public:
    KeyActionController(uint8_t numKeys, KeyStateController &keyStateController);

    KeyLayer &addLayer();

    void switchBaseLayer(KeyLayer &layer);

    OverlayLayerReference addOverlayLayer(KeyLayer &layer);

    void removeOverlayLayer(OverlayLayerReference reference);

    [[nodiscard]] ListenerPriority getPriority() const override;

    void onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) override;
};
