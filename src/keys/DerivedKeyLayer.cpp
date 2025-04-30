#include "DerivedKeyLayer.h"

#include <utility>

IKeyAction* DerivedKeyLayer::getAction(const uint8_t keyId) const {
    if (const auto action = BasicKeyLayer::getAction(keyId)) {
        return action;
    }
    return base.getAction(keyId);
}

DerivedKeyLayer::DerivedKeyLayer(const uint8_t numKeys, KeyLayer& base, std::shared_ptr<IRGBEffect> effect)
    : BasicKeyLayer(numKeys, std::move(effect)), base(base) {}
