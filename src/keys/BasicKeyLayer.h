#pragma once

#include <memory>

#include "KeyLayer.h"
#include "action/IKeyAction.h"

class BasicKeyLayer : public KeyLayer {
    std::unique_ptr<std::unique_ptr<IKeyAction>[]> actions;

public:
    explicit BasicKeyLayer(uint8_t numKeys);

    template <KeyActionType T>
    void setAction(const uint8_t keyId, T action) {
        actions[keyId] = std::make_unique<T>(std::move(action));
    }

    [[nodiscard]] IKeyAction* getAction(uint8_t keyId) const override;
};
