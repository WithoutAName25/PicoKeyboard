#ifndef PICOKEYBOARD_KEYBOARDCONTROLLER_H
#define PICOKEYBOARD_KEYBOARDCONTROLLER_H

#include <memory>
#include "KeyLayer.h"
#include "KeyOverlayLayer.h"

class KeyboardController : IKeyStateListener {
private:
    std::vector<KeyLayer> layers;
    uint16_t numLayers;
    uint8_t numKeys;
    KeyLayer *currentBaseLayer;
    std::unique_ptr<KeyOverlayLayer> overlayLayerStack;

    IKeyAction *getAction(uint8_t keyId, absolute_time_t timestamp);

    void removeOverlayLayer(KeyOverlayLayer *last, KeyOverlayLayer *layer);

    static bool isValid(KeyOverlayLayer *overlayLayer, absolute_time_t timestamp);

public:
    KeyboardController(uint8_t numKeys, KeyStateManager manager);

    KeyLayer &addLayer();

    void switchBaseLayer(uint16_t layerId);

    void addOverlayLayer(uint16_t layerId, uint8_t activatedByKeyId, absolute_time_t timestamp);

    void addSingleUseOverlayLayer(uint16_t layerId, absolute_time_t timestamp);

    void addExpiringOverlayLayer(uint16_t layerId, absolute_time_t timestamp, absolute_time_t expirationTime);

    [[nodiscard]] ListenerPriority getPriority() const override;

    void onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) override;
};

#endif //PICOKEYBOARD_KEYBOARDCONTROLLER_H
