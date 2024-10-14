#include "KeyActionController.h"

IKeyAction *KeyActionController::getAction(uint8_t keyId) {
    for (KeyLayer *overlayLayer: activeOverlayLayers) {
        IKeyAction *action = overlayLayer->getAction(keyId);
        if (action != nullptr) {
            return action;
        }
    }
    if (currentBaseLayer == nullptr) return nullptr;
    return currentBaseLayer->getAction(keyId);
}

KeyActionController::KeyActionController(uint8_t numKeys, KeyStateController &keyStateController)
        : keyStateController(keyStateController),
          layers(),
          numKeys(numKeys),
          currentBaseLayer(nullptr),
          activeOverlayLayers() {
    keyStateController.addKeyStateListener(this);
}

KeyLayer &KeyActionController::addLayer() {
    layers.emplace_back(numKeys);
    return layers.back();
}

void KeyActionController::switchBaseLayer(KeyLayer &layer) {
    currentBaseLayer = &layer;
}

OverlayLayerReference KeyActionController::addOverlayLayer(KeyLayer &layer) {
    activeOverlayLayers.emplace_back(&layer);
    return --activeOverlayLayers.end();
}

void KeyActionController::removeOverlayLayer(OverlayLayerReference reference) {
    activeOverlayLayers.erase(reference);
}

ListenerPriority KeyActionController::getPriority() const {
    return ListenerPriority::WITH_KEYBOARD_CONTROLLER;
}

void KeyActionController::onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) {
    if (!state.isPressed) return;

    IKeyAction *action = getAction(keyId);
    if (action != nullptr) {
        action->execute(keyId, &state, timestamp);
    }
}
