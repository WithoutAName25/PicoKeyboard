#include "BasicKeyLayer.h"

BasicKeyLayer::BasicKeyLayer(const uint8_t numKeys)
    : KeyLayer(numKeys), actions(std::make_unique<std::unique_ptr<IKeyAction>[]>(numKeys)) {}

IKeyAction* BasicKeyLayer::getAction(const uint8_t keyId) const {
    return actions[keyId].get();
}
