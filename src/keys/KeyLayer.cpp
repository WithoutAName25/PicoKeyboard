#include "KeyLayer.h"

KeyLayer::KeyLayer(uint8_t numKeys)
        : actions(std::make_unique<std::unique_ptr<IKeyAction>[]>(numKeys)),
          numKeys(numKeys) {
}

IKeyAction *KeyLayer::getAction(uint8_t keyId) {
    return actions[keyId].get();
}
