#include "KeyActionController.h"

IKeyAction* KeyActionController::getAction(const uint8_t keyId) const {
    for (const KeyLayer* overlayLayer : activeOverlayLayers) {
        if (IKeyAction* action = overlayLayer->getAction(keyId); action != nullptr) {
            return action;
        }
    }
    if (currentBaseLayer == nullptr) return nullptr;
    return currentBaseLayer->getAction(keyId);
}

KeyActionController::KeyActionController(const uint8_t numKeys, KeyStateController& keyStateController)
    : keyStateController(keyStateController),
      numKeys(numKeys),
      currentBaseLayer(nullptr) {
    keyStateController.addKeyStateListener(this);
}

void KeyActionController::switchBaseLayer(KeyLayer& layer) {
    currentBaseLayer = &layer;
}

OverlayLayerReference KeyActionController::addOverlayLayer(KeyLayer& layer) {
    activeOverlayLayers.emplace_front(&layer);
    return activeOverlayLayers.begin();
}

void KeyActionController::removeOverlayLayer(const OverlayLayerReference reference) {
    activeOverlayLayers.erase(reference);
}

ListenerPriority KeyActionController::getPriority() const {
    return ListenerPriority::WITH_KEYBOARD_CONTROLLER;
}

void KeyActionController::onKeyStateChange(const uint8_t keyId, const KeyState& state,
                                           const absolute_time_t timestamp) {
    if (!state.isPressed) return;

    if (const auto action = getAction(keyId); action != nullptr) {
        action->execute(keyId, &state, timestamp);
    }
}
