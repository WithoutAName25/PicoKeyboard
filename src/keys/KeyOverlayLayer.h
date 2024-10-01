#pragma once

#include "KeyLayer.h"

enum LifetimeType {
    WHILE_HELD,
    SINGLE_USE,
    EXPIRING,
};

struct KeyOverlayLayer {
    KeyLayer *layer;
    LifetimeType lifetimeType;
    uint8_t activatedByKeyId;
    absolute_time_t activationTime;
    absolute_time_t expirationTime;
    std::unique_ptr<KeyOverlayLayer> next;
};
