#include "OverlayLayerAction.h"

extern KeyActionController keyActionController;

[[maybe_unused]] OverlayLayerAction::OverlayLayerAction(KeyLayer &layer) : layer(layer) {}

void OverlayLayerAction::press() {
    reference = keyActionController.addOverlayLayer(layer);
}

void OverlayLayerAction::release() {
    keyActionController.removeOverlayLayer(reference);
}
