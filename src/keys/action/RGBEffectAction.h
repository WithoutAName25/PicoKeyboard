#pragma once
#include "IKeyAction.h"
#include <rgb.h>

class RGBEffectAction final : public IKeyAction {
    std::shared_ptr<IRGBEffect> effect;
    uint64_t fadeTime;

    void execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) override;

public:
    explicit RGBEffectAction(const std::shared_ptr<IRGBEffect> &effect, uint64_t fadeTime = 0);

    template <RGBEffectType T>
    explicit RGBEffectAction(T effect, const uint64_t fadeTime = 0)
        : effect(std::make_shared<T>(std::move(effect))), fadeTime(fadeTime) {}
};
