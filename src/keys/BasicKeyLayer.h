#pragma once

#include <memory>
#include <rgb.h>

#include "KeyLayer.h"
#include "action/IKeyAction.h"

class BasicKeyLayer : public KeyLayer {
    std::unique_ptr<std::unique_ptr<IKeyAction>[]> actions;
    std::shared_ptr<IRGBEffect> effect;
    bool enabled = false;
    OverlayEffectReference overlayEffect;

protected:
    [[nodiscard]] IKeyAction *getAction(uint8_t keyId) const override;

    void enable() override;
    void disable() override;

public:
    explicit BasicKeyLayer(uint8_t numKeys, std::shared_ptr<IRGBEffect> effect = nullptr);

    template <KeyActionType T> void setAction(const uint8_t keyId, T action) {
        actions[keyId] = std::make_unique<T>(std::move(action));
    }
};
