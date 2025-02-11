#pragma once

#include <memory>
#include "action/IKeyAction.h"

class KeyLayer {
    std::unique_ptr<std::unique_ptr<IKeyAction>[]> actions;
    uint8_t numKeys;

public:
    explicit KeyLayer(uint8_t numKeys);

    template<typename T>
    void setAction(const uint8_t keyId, T action) {
        actions[keyId] = std::make_unique<T>(std::move(action));
    }

    [[nodiscard]] IKeyAction *getAction(uint8_t keyId) const;
};
