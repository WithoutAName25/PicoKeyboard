#include "KeyActionController.h"

IKeyAction *KeyActionController::getAction(uint8_t keyId, absolute_time_t timestamp) {
    KeyOverlayLayer *last = nullptr;
    for (KeyOverlayLayer *layer = overlayLayerStack.get(); layer != nullptr; layer = layer->next.get()) {
        if (!isValid(layer, timestamp)) {
            KeyOverlayLayer *next = layer->next.get();
            removeOverlayLayer(last, layer);
            if (next == nullptr) break;
            layer = next;
        }
        IKeyAction *action = layer->layer->getAction(keyId);
        if (action != nullptr) {
            if (layer->lifetimeType == SINGLE_USE)
                removeOverlayLayer(last, layer);
            return action;
        }
        last = layer;
    }
    if (currentBaseLayer == nullptr) return nullptr;
    return currentBaseLayer->getAction(keyId);
}

void KeyActionController::removeOverlayLayer(KeyOverlayLayer *last, KeyOverlayLayer *layer) {
    if (last != nullptr) {
        last->next = std::move(layer->next);
    } else {
        overlayLayerStack = std::move(layer->next);
    }
}

bool KeyActionController::isValid(KeyOverlayLayer *overlayLayer, absolute_time_t timestamp) {
    switch (overlayLayer->lifetimeType) {
        case WHILE_HELD: {
            KeyState &state = keyStateController.getKeyState(overlayLayer->activatedByKeyId);
            return state.isPressed && state.pressTime <= overlayLayer->activationTime;
        }
        case SINGLE_USE:
            return true;
        case EXPIRING:
            return overlayLayer->expirationTime > timestamp;
    }
    return false;
}

KeyActionController::KeyActionController(uint8_t numKeys, KeyStateController &keyStateController)
        : keyStateController(keyStateController),
          layers(),
          numKeys(numKeys),
          currentBaseLayer(nullptr),
          overlayLayerStack(nullptr) {
    keyStateController.addKeyStateListener(this);
}

KeyLayer &KeyActionController::addLayer() {
    layers.emplace_back(numKeys);
    return layers.back();
}

void KeyActionController::switchBaseLayer(KeyLayer &layer) {
    currentBaseLayer = &layer;
    overlayLayerStack = nullptr;
}

void KeyActionController::addOverlayLayer(KeyLayer &layer, uint8_t activatedByKeyId, absolute_time_t timestamp) {
    overlayLayerStack = std::make_unique<KeyOverlayLayer>(KeyOverlayLayer{
            &layer,
            WHILE_HELD,
            activatedByKeyId,
            timestamp,
            0,
            std::move(overlayLayerStack)
    });
}

void KeyActionController::addSingleUseOverlayLayer(KeyLayer &layer, absolute_time_t timestamp) {
    overlayLayerStack = std::make_unique<KeyOverlayLayer>(KeyOverlayLayer{
            &layer,
            SINGLE_USE,
            0,
            timestamp,
            0,
            std::move(overlayLayerStack)
    });
}

void KeyActionController::addExpiringOverlayLayer(KeyLayer &layer, absolute_time_t timestamp,
                                                  absolute_time_t expirationTime) {
    overlayLayerStack = std::make_unique<KeyOverlayLayer>(KeyOverlayLayer{
            &layer,
            EXPIRING,
            0,
            timestamp,
            expirationTime,
            std::move(overlayLayerStack)
    });
}

ListenerPriority KeyActionController::getPriority() const {
    return ListenerPriority::WITH_KEYBOARD_CONTROLLER;
}

void KeyActionController::onKeyStateChange(uint8_t keyId, KeyState &state, absolute_time_t timestamp) {
    if (!state.isPressed) return;

    IKeyAction *action = getAction(keyId, timestamp);
    if (action != nullptr) {
        action->execute(keyId, &state, timestamp);
    }
}
