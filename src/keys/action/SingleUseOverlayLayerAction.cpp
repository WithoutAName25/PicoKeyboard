#include "SingleUseOverlayLayerAction.h"

extern KeyActionController keyActionController;
extern KeyStateController keyStateController;

void SingleUseOverlayLayerAction::onKeyStateChange(const uint8_t keyId, const KeyState& state, absolute_time_t timestamp) {
    if (layer.getAction(keyId) != nullptr) {
        keyActionController.removeOverlayLayer(reference);
        keyStateController.removeKeyStateListener(listenerReference);
    }
}

[[maybe_unused]] SingleUseOverlayLayerAction::SingleUseOverlayLayerAction(KeyLayer &layer) : layer(layer) {}

void SingleUseOverlayLayerAction::execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) {
    reference = keyActionController.addOverlayLayer(layer);
    listenerReference = keyStateController.addKeyStateListener(this);
}
