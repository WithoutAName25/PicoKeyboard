#pragma once

#include <memory>
#include "action/IKeyAction.h"

class KeyLayer {
private:
    uint16_t id;
    std::unique_ptr<std::unique_ptr<IKeyAction>[]> actions;
    uint8_t numKeys;

public:
    explicit KeyLayer(uint16_t id, uint8_t numKeys);

    void setAction(uint8_t keyId, std::unique_ptr<IKeyAction> action);

    IKeyAction *getAction(uint8_t keyId);
};
