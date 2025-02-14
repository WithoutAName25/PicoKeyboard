#pragma once
#include "IKeyAction.h"
#include <rgb.h>

class RGBEffectAction final : public IKeyAction {
    std::shared_ptr<IRGBEffect> effect;

    void execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) override;

public:
    explicit RGBEffectAction(const std::shared_ptr<IRGBEffect>& effect);

    template <RGBEffectType T>
    explicit RGBEffectAction(T effect) {
        this->effect = std::make_shared<T>(effect);
    }
};
