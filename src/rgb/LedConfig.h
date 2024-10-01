#pragma once

#include "pico/stdlib.h"

struct LedConfig {
    uint8_t hwNumber;
    uint8_t id;
    float positionX;
    float positionY;
    bool isKeyLed;
    uint8_t associatedKeyId;

    LedConfig(uint8_t hwNumber, uint8_t id, float positionX, float positionY)
            : hwNumber(hwNumber), id(id),
              positionX(positionX), positionY(positionY),
              isKeyLed(false), associatedKeyId(0) {}

    LedConfig(uint8_t hwNumber, uint8_t id, float positionX, float positionY, uint8_t associatedKeyId)
            : hwNumber(hwNumber), id(id),
              positionX(positionX), positionY(positionY),
              isKeyLed(true), associatedKeyId(associatedKeyId) {}
};
