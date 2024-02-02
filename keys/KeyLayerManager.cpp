#include "KeyLayerManager.h"

void KeyLayerManager::pushLayer(uint8_t layerID, uint8_t activatedByKey) {
    currentLayer = std::make_unique<LayerStack>(LayerStack{
            .current = layers.at(layerID).get(),
            .activatedByKey = activatedByKey,
            .previous = std::move(currentLayer)
    });
}

void KeyLayerManager::popLayer() {
    if (currentLayer->previous) {
        currentLayer = std::move(currentLayer->previous);
    } else {
        currentLayer.reset();
    }
}

KeyAction *KeyLayerManager::getAction(uint8_t pressedKey) {
    KeyAction *action = nullptr;
    for (LayerStack *it = currentLayer.get(); it != nullptr; it = it->previous.get()) {
        if (it->activatedByKey == pressedKey) {
            return nullptr;
        }
        if (action == nullptr && it->current[pressedKey].type != PREVIOUS_LAYER_ACTION) {
            action = &it->current[pressedKey];
        }
    }
    return action;
}

void KeyLayerManager::tickCurrentLayer() {
    for (LayerStack *current = currentLayer.get(); current != nullptr; current = current->previous.get()) {
        if (current->activatedByKey != NO_KEY && !keys->isActive(current->activatedByKey)) {
            currentLayer = std::move(current->previous);
        }
    }
    for (uint8_t key = 0; key < keys->getNumKeys(); ++key) {
        if (keys->isActive(key)) {
            KeyAction *action = getAction(key);
            if (action != nullptr) {
                switch (action->type) {
                    case ROOT_LAYER_SWITCH:
                        setRootLayer(action->data);
                        break;
                    case ENABLE_OVERLAY_LAYER:
                        pushLayer(action->data, key);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void KeyLayerManager::tickPressedKeys() {
    pressedKeys.clear();
    for (uint8_t key = 0; key < keys->getNumKeys(); ++key) {
        if (keys->isActive(key)) {
            KeyAction *action = getAction(key);
            if (action != nullptr && action->type == KEY_PRESS) {
                pressedKeys.push_back(action->data);
            }
        }
    }
}

uint8_t KeyLayerManager::addLayer(std::unique_ptr<KeyAction[]> layer) {
    layers.push_back(std::move(layer));
    return layers.size() - 1;
}

void KeyLayerManager::setRootLayer(uint8_t layerID) {
    currentLayer = std::make_unique<LayerStack>();
    currentLayer->current = layers[layerID].get();
}

void KeyLayerManager::tick() {
    tickCurrentLayer();
    tickPressedKeys();
}

std::vector<uint8_t> KeyLayerManager::getPressedKeys() {
    return pressedKeys;
}