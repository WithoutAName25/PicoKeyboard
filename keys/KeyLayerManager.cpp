#include "KeyLayerManager.h"

void KeyLayerManager::pushLayer(uint8_t layerID, uint8_t activatedByKey) {
    currentLayer = std::make_unique<LayerStack>(LayerStack{
            .current = layers.at(layerID).get(),
            .wasActivatedByKey = activatedByKey,
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

KeyAction *KeyLayerManager::getAction(uint8_t pressedKey) const {
    KeyAction *action = nullptr;
    for (LayerStack *it = currentLayer.get(); it != nullptr; it = it->previous.get()) {
        if (it->wasActivatedByKey == pressedKey) {
            return nullptr;
        }
        if (action == nullptr && it->current[pressedKey].type != PREVIOUS_LAYER_ACTION) {
            action = &it->current[pressedKey];
        }
    }
    return action;
}

void KeyLayerManager::tickCurrentLayer() {
    for (LayerStack *current = currentLayer.get(); current != nullptr;) {
        if (current->wasActivatedByKey != NO_KEY && !keys->isActive(current->wasActivatedByKey)) {
            currentLayer = std::move(current->previous);
            current = currentLayer.get();
        } else {
            current = current->previous.get();
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
    customActions.clear();
    for (uint8_t key = 0; key < keys->getNumKeys(); ++key) {
        if (keys->isActive(key)) {
            KeyAction *action = getAction(key);
            if (action != nullptr) {
                switch (action->type) {
                    case KEY_PRESS:
                        pressedKeys.push_back(action->data);
                        break;
                    case SWITCH_MODE:
                        mode = static_cast<Mode>(action->data);
                        break;
                    case CUSTOM:
                        customActions.push_back(action->data);
                    default:
                        break;
                }
            }
        }
    }
}

LayerConfig KeyLayerManager::addLayer() {
    std::unique_ptr<KeyAction[]> actions(new KeyAction[keys->getNumKeys()]);
    LayerConfig config = {
            .id = static_cast<uint8_t>(layers.size()),
            .actions = actions.get()
    };
    layers.push_back(std::move(actions));
    return config;
}

void KeyLayerManager::setRootLayer(uint8_t layerID) {
    if (currentRootLayerID == layerID) return;

    currentRootLayerID = layerID;
    currentLayer = std::make_unique<LayerStack>();
    currentLayer->current = layers[layerID].get();
}

void KeyLayerManager::tick() {
    tickCurrentLayer();
    tickPressedKeys();
}

Mode KeyLayerManager::getMode() const {
    return mode;
}

std::vector<uint8_t> KeyLayerManager::getPressedKeys() const {
    return pressedKeys;
}
