#pragma once

#include <memory>

#include "action/IKeyAction.h"

class KeyLayer {
protected:
    uint8_t numKeys;

public:
    explicit KeyLayer(uint8_t numKeys);
    virtual ~KeyLayer() = default;

    [[nodiscard]] virtual IKeyAction* getAction(uint8_t keyId) const = 0;
};
