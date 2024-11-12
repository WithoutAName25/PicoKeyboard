#include "OverlayLayerAction.h"

extern KeyActionController keyActionController;

[[maybe_unused]] OverlayLayerAction::OverlayLayerAction(KeyLayer &layer) : layer(layer) {}

void OverlayLayerAction::press(absolute_time_t timestamp) {
    reference = keyActionController.addOverlayLayer(layer);
}

void OverlayLayerAction::release(absolute_time_t timestamp) {
    keyActionController.removeOverlayLayer(reference);
}
