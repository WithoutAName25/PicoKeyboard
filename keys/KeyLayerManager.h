#ifndef PICOKEYBOARD_KEYLAYERMANAGER_H
#define PICOKEYBOARD_KEYLAYERMANAGER_H

#include <vector>
#include "pico/stdlib.h"
#include "KeyManager.h"

#define NO_KEY 255
enum KeyActionType : uint8_t {
    NO_ACTION,
    KEY_PRESS,
    SWITCH_MODE,
    CUSTOM,
    ROOT_LAYER_SWITCH,
    ENABLE_OVERLAY_LAYER,
    PREVIOUS_LAYER_ACTION,
};

enum Mode : uint8_t {
    MODE_DEFAULT,
    MODE_PONG
};

struct KeyAction {
    KeyActionType type = PREVIOUS_LAYER_ACTION;
    uint8_t data = 0;
};

struct LayerStack {
    KeyAction *current;
    uint8_t wasActivatedByKey = NO_KEY;
    std::unique_ptr<LayerStack> previous;
};

struct LayerConfig {
    uint8_t id;
    KeyAction *actions;
};

class KeyLayerManager {
private:
    KeyManager *keys;
    std::vector<std::unique_ptr<KeyAction[]>> layers;
    std::unique_ptr<LayerStack> currentLayer;
    uint8_t currentRootLayerID;
    std::vector<uint8_t> pressedKeys;
    std::vector<uint8_t> customActions;
    Mode mode = MODE_DEFAULT;

    void pushLayer(uint8_t layerID, uint8_t activatedByKey);

    void popLayer();

    [[nodiscard]] KeyAction *getAction(uint8_t pressedKey) const;

    void tickCurrentLayer();

    void tickPressedKeys();

public:
    // Constructor implementation
    explicit KeyLayerManager(KeyManager *keyMgr) : keys(keyMgr), currentRootLayerID(255) {}

    LayerConfig addLayer();

    void setRootLayer(uint8_t layerID);

    void tick();

    [[nodiscard]] Mode getMode() const;

    [[nodiscard]] std::vector<uint8_t> getPressedKeys() const;
};

#endif //PICOKEYBOARD_KEYLAYERMANAGER_H