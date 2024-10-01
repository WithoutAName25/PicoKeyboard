#include "KeyLayer.h"

KeyLayer::KeyLayer(uint16_t id, uint8_t numKeys)
        : id(id),
          actions(std::make_unique<std::unique_ptr<IKeyAction>[]>(numKeys)),
          numKeys(numKeys) {
}

void KeyLayer::setAction(uint8_t keyId, std::unique_ptr<IKeyAction> action) {
    actions[keyId] = std::move(action);
}

IKeyAction *KeyLayer::getAction(uint8_t keyId) {
    return actions[keyId].get();
}
