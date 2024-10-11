#pragma once

#include <memory>
#include "KeyLayer.h"
#include "KeyOverlayLayer.h"

class KeyActionController : IKeyStateListener {
private:
    KeyStateController &keyStateController;
    std::vector<KeyLayer> layers;
    uint8_t numKeys;
    KeyLayer *currentBaseLayer;
    std::unique_ptr<KeyOverlayLayer> overlayLayerStack;

    IKeyAction *getAction(uint8_t keyId, absolute_time_t timestamp);

    void removeOverlayLayer(KeyOverlayLayer *last, KeyOverlayLayer *layer);

    bool isValid(KeyOverlayLayer *overlayLayer, absolute_time_t timestamp);

public:
    KeyActionController(uint8_t numKeys, KeyStateController &keyStateController);

    KeyLayer &addLayer();

    void switchBaseLayer(KeyLayer &layer);

    void addOverlayLayer(KeyLayer &layer, uint8_t activatedByKeyId, absolute_time_t timestamp);

    void addSingleUseOverlayLayer(KeyLayer &layer, absolute_time_t timestamp);

    void addExpiringOverlayLayer(KeyLayer &layer, absolute_time_t timestamp, absolute_time_t expirationTime);

    [[nodiscard]] ListenerPriority getPriority() const override;

    void onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) override;
};
