#pragma once

#include <vector>
#include <memory>
#include "SequenceEffect.h"

class TypewriterEffect {
    struct KeyEvent {
        enum class Type { PRESS, RELEASE };

        uint8_t keyId;
        uint64_t timeMs;
        Type type;

        KeyEvent(const uint8_t keyId, const uint64_t timeMs, const Type type, bool)
            : keyId(keyId), timeMs(timeMs), type(type) {}
    };

    uint16_t keyPressMs;
    uint16_t holdPreTimeMs;
    uint16_t holdPostTimeMs;
    uint16_t keyGapMs;
    uint16_t fadeInMs;
    uint16_t fadeOutMs;
    Color color;

    std::vector<KeyEvent> keyEvents;
    uint64_t currentTimeMs;
    bool needsGap = false;

    std::vector<SequenceStep> generateSequence();

public:
    explicit TypewriterEffect(
        uint16_t keyPressMs = 250,
        uint16_t holdPreTimeMs = 500,
        uint16_t holdPostTimeMs = 350,
        uint16_t keyGapMs = 350,
        uint16_t fadeInMs = 0,
        uint16_t fadeOutMs = 500,
        const Color& color = Color::FromRGB(0, 0, 255)
    );

    TypewriterEffect& pressKey(uint8_t keyId);

    TypewriterEffect& delay(uint64_t delayMs);

    TypewriterEffect& holdKey(uint8_t keyId);
    TypewriterEffect& holdKeysSimultaneously(const std::vector<uint8_t>& keyIds);

    TypewriterEffect& releaseKey(uint8_t keyId);
    TypewriterEffect& releaseKeysSimultaneously(const std::vector<uint8_t>& keyIds);

    TypewriterEffect& typeKeys(const std::vector<uint8_t>& keyIds);

    std::shared_ptr<SequenceEffect> build(absolute_time_t startTime, bool looping = true);
};
