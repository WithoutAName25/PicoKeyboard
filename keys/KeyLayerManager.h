#ifndef PICOKEYBOARD_KEYLAYERMANAGER_H
#define PICOKEYBOARD_KEYLAYERMANAGER_H

#include <vector>
#include "pico/stdlib.h"
#include "KeyManager.h"

#define NO_KEY 255
enum KeyActionType {
    NO_ACTION,
    KEY_PRESS,
    ROOT_LAYER_SWITCH,
    ENABLE_OVERLAY_LAYER,
    PREVIOUS_LAYER_ACTION,
};

struct KeyAction {
    KeyActionType type = NO_ACTION;
    uint8_t data = 0;
};

struct LayerStack {
    KeyAction *current;
    uint8_t activatedByKey = NO_KEY;
    std::unique_ptr<LayerStack> previous;
};

class KeyLayerManager {
private:
public:
    KeyManager *keys;
    std::vector<std::unique_ptr<KeyAction[]>> layers;
    std::unique_ptr<LayerStack> currentLayer;
    std::vector<uint8_t> pressedKeys;

    void pushLayer(uint8_t layerID, uint8_t activatedByKey);

    void popLayer();

    KeyAction *getAction(uint8_t pressedKey);

    void tickCurrentLayer();

    void tickPressedKeys();

public:
    // Constructor implementation
    explicit KeyLayerManager(KeyManager *keyMgr) : keys(keyMgr) {}

    uint8_t addLayer(std::unique_ptr<KeyAction[]> layer);

    void setRootLayer(uint8_t layerID);

    void tick();

    std::vector<uint8_t> getPressedKeys();
};

#endif //PICOKEYBOARD_KEYLAYERMANAGER_H