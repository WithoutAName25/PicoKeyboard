#include "SingleUseOverlayLayerAction.h"

extern KeyActionController keyActionController;
extern KeyStateController keyStateController;

void SingleUseOverlayLayerAction::onKeyStateChange(const uint8_t keyId, const KeyState& state,
                                                   absolute_time_t timestamp) {
    if (!isActive) return;
    if (layer.getAction(keyId) == nullptr) return;

    keyActionController.removeOverlayLayer(reference);
    keyStateController.removeKeyStateListener(listenerReference);
    isActive = false;
}

[[maybe_unused]] SingleUseOverlayLayerAction::SingleUseOverlayLayerAction(KeyLayer& layer) : layer(layer) {}

void SingleUseOverlayLayerAction::execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) {
    if (isActive) {
        keyActionController.removeOverlayLayer(reference);
        reference = keyActionController.addOverlayLayer(layer);
        return;
    }

    isActive = true;
    reference = keyActionController.addOverlayLayer(layer);
    listenerReference = keyStateController.addKeyStateListener(this);
}
