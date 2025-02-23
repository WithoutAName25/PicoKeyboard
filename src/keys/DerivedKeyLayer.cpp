#include "DerivedKeyLayer.h"

DerivedKeyLayer::DerivedKeyLayer(const uint8_t numKeys, KeyLayer& base): BasicKeyLayer(numKeys), base(base) {}

IKeyAction* DerivedKeyLayer::getAction(const uint8_t keyId) const {
    if (const auto action = BasicKeyLayer::getAction(keyId)) {
        return action;
    }
    return base.getAction(keyId);
}
