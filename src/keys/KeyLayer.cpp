#include "KeyLayer.h"

KeyLayer::KeyLayer(const uint8_t numKeys)
        : actions(std::make_unique<std::unique_ptr<IKeyAction>[]>(numKeys)),
          numKeys(numKeys) {
}

IKeyAction *KeyLayer::getAction(const uint8_t keyId) const {
    return actions[keyId].get();
}
