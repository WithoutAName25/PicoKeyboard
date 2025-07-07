#pragma once

#include "IRGBEffect.h"
#include <vector>

struct SequenceStep {
    uint8_t id;
    bool idIsLedId;
    uint64_t startTimeMs;
    Color color;
    uint16_t durationMs;
    uint16_t fadeInMs;
    uint16_t fadeOutMs;

    SequenceStep(const uint8_t id, const bool idIsLedId, const uint64_t startTimeMs, const Color &color,
                 const uint16_t durationMs, const uint16_t fadeInMs, const uint16_t fadeOutMs)
        : id(id), idIsLedId(idIsLedId), startTimeMs(startTimeMs), color(color), durationMs(durationMs),
          fadeInMs(fadeInMs), fadeOutMs(fadeOutMs) {}
};

class SequenceEffect final : public IRGBEffect {
    std::vector<SequenceStep> sequence;
    uint64_t totalDurationMs;
    bool looping;
    absolute_time_t startTime;

    void serialize(InterDeviceCommunicator &communicator) override;
    Color getColor(LedConfig &led, absolute_time_t timestamp) override;

    Color calculateLedColor(const LedConfig &led, uint64_t timeInSequenceMs);

public:
    SequenceEffect(const std::vector<SequenceStep> &sequence, absolute_time_t startTime, bool looping = true);

    explicit SequenceEffect(InterDeviceCommunicator &communicator);
};
