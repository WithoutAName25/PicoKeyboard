#pragma once
#include "BasicKeyLayer.h"
#include "KeyLayer.h"

class DerivedKeyLayer final : public BasicKeyLayer {
    KeyLayer& base;

    [[nodiscard]] IKeyAction* getAction(uint8_t keyId) const override;

public:
    DerivedKeyLayer(uint8_t numKeys, KeyLayer& base, std::shared_ptr<IRGBEffect> effect = nullptr);
};
