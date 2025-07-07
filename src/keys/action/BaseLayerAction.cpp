#include "BaseLayerAction.h"
#include "../KeyActionController.h"

extern KeyActionController keyActionController;

[[maybe_unused]] BaseLayerAction::BaseLayerAction(KeyLayer &layer) : layer(layer) {}

void BaseLayerAction::execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) {
    keyActionController.switchBaseLayer(layer);
}
