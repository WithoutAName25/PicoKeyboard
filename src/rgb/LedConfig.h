#pragma once

struct LedConfig {
    uint8_t hwNumber;
    uint8_t id;
    float positionX;
    float positionY;
    bool isKeyLed;
    uint8_t associatedKeyId;

    LedConfig(const uint8_t hwNumber, const uint8_t id, const float positionX, const float positionY)
        : hwNumber(hwNumber), id(id), positionX(positionX), positionY(positionY), isKeyLed(false), associatedKeyId(0) {}

    LedConfig(const uint8_t hwNumber, const uint8_t id, const float positionX, const float positionY,
              const uint8_t associatedKeyId)
        : hwNumber(hwNumber), id(id), positionX(positionX), positionY(positionY), isKeyLed(true),
          associatedKeyId(associatedKeyId) {}
};
