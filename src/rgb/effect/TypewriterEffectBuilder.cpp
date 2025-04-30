#include "TypewriterEffectBuilder.h"

#include <map>

TypewriterEffect::TypewriterEffect(
    const uint16_t keyPressMs,
    const uint16_t holdPreTimeMs,
    const uint16_t holdPostTimeMs,
    const uint16_t keyGapMs,
    const uint16_t fadeInMs,
    const uint16_t fadeOutMs,
    const Color& color
) : keyPressMs(keyPressMs),
    holdPreTimeMs(holdPreTimeMs),
    holdPostTimeMs(holdPostTimeMs),
    keyGapMs(keyGapMs),
    fadeInMs(fadeInMs),
    fadeOutMs(fadeOutMs),
    color(color),
    currentTimeMs(fadeInMs) {}

TypewriterEffect& TypewriterEffect::pressKey(uint8_t keyId) {
    if (needsGap) {
        currentTimeMs += keyGapMs;
        needsGap = false;
    }

    keyEvents.emplace_back(keyId, currentTimeMs, KeyEvent::Type::PRESS, false);
    keyEvents.emplace_back(keyId, currentTimeMs + keyPressMs, KeyEvent::Type::RELEASE, false);

    currentTimeMs += keyPressMs;

    needsGap = true;

    return *this;
}

TypewriterEffect& TypewriterEffect::delay(const uint64_t delayMs) {
    currentTimeMs += delayMs;
    return *this;
}

TypewriterEffect& TypewriterEffect::holdKey(uint8_t keyId) {
    if (needsGap) {
        currentTimeMs += keyGapMs;
        needsGap = false;
    }

    keyEvents.emplace_back(keyId, currentTimeMs, KeyEvent::Type::PRESS, true);

    currentTimeMs += holdPreTimeMs;

    return *this;
}

TypewriterEffect& TypewriterEffect::holdKeysSimultaneously(const std::vector<uint8_t>& keyIds) {
    if (needsGap) {
        currentTimeMs += keyGapMs;
        needsGap = false;
    }

    for (const uint8_t keyId : keyIds) {
        keyEvents.emplace_back(keyId, currentTimeMs, KeyEvent::Type::PRESS, true);
    }

    currentTimeMs += holdPreTimeMs;

    return *this;
}

TypewriterEffect& TypewriterEffect::releaseKey(uint8_t keyId) {
    currentTimeMs += holdPostTimeMs;

    keyEvents.emplace_back(keyId, currentTimeMs, KeyEvent::Type::RELEASE, true);

    return *this;
}

TypewriterEffect& TypewriterEffect::releaseKeysSimultaneously(const std::vector<uint8_t>& keyIds) {
    currentTimeMs += holdPostTimeMs;

    for (const uint8_t keyId : keyIds) {
        keyEvents.emplace_back(keyId, currentTimeMs, KeyEvent::Type::RELEASE, true);
    }

    return *this;
}

TypewriterEffect& TypewriterEffect::typeKeys(const std::vector<uint8_t>& keyIds) {
    for (const uint8_t keyId : keyIds) {
        pressKey(keyId);
    }
    return *this;
}

std::shared_ptr<SequenceEffect> TypewriterEffect::build(absolute_time_t startTime, bool looping) {
    std::vector<SequenceStep> sequence = generateSequence();
    return std::make_shared<SequenceEffect>(sequence, startTime, looping);
}

std::vector<SequenceStep> TypewriterEffect::generateSequence() {
    std::vector<SequenceStep> steps;

    std::map<uint8_t, uint64_t> pressStartTimes;

    for (const auto& event : keyEvents) {
        switch (event.type) {
        case KeyEvent::Type::PRESS:
            pressStartTimes[event.keyId] = event.timeMs;
            break;
        case KeyEvent::Type::RELEASE:
            steps.emplace_back(
                event.keyId,
                false,
                pressStartTimes[event.keyId] - fadeInMs,
                color,
                event.timeMs - pressStartTimes[event.keyId],
                fadeInMs,
                fadeOutMs
            );
            break;
        }
    }

    return steps;
}
