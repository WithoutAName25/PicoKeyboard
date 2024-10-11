#pragma once

#include <memory>
#include "action/IKeyAction.h"

class KeyLayer {
private:
    std::unique_ptr<std::unique_ptr<IKeyAction>[]> actions;
    uint8_t numKeys;

public:
    explicit KeyLayer(uint8_t numKeys);

    template<typename T>
    void setAction(uint8_t keyId, T action) {
        actions[keyId] = std::make_unique<T>(std::move(action));
    };

    IKeyAction *getAction(uint8_t keyId);
};
