#include "SequenceEffect.h"

#include <algorithm>
#include <communication.h>

void SequenceEffect::serialize(InterDeviceCommunicator &communicator) {
    IRGBEffect::serialize(communicator);

    communicator.send32(sequence.size());

    for (const auto &step : sequence) {
        communicator.send(step.id);
        communicator.send(step.idIsLedId);
        communicator.send64(step.startTimeMs);
        step.color.serialize(communicator);
        communicator.send16(step.durationMs);
        communicator.send16(step.fadeInMs);
        communicator.send16(step.fadeOutMs);
    }

    communicator.send(looping);

    communicator.send64(startTime);
}

Color SequenceEffect::getColor(LedConfig &led, const absolute_time_t timestamp) {
    if (timestamp < startTime || (!looping && startTime + totalDurationMs * 1000 < timestamp))
        return Color::None();

    const uint64_t elapsedMs = absolute_time_diff_us(startTime, timestamp) / 1000;
    const uint64_t timeInSequenceMs = looping ? (elapsedMs % totalDurationMs) : std::min(elapsedMs, totalDurationMs);

    return calculateLedColor(led, timeInSequenceMs);
}

Color SequenceEffect::calculateLedColor(const LedConfig &led, const uint64_t timeInSequenceMs) {
    Color resultColor = Color::Black();

    for (const auto &step : sequence) {
        if (step.idIsLedId && step.id != led.id)
            continue;
        if (!step.idIsLedId && led.isKeyLed && step.id != led.associatedKeyId)
            continue;

        const uint64_t stepStartMs = step.startTimeMs;
        const uint64_t fadeInEndMs = stepStartMs + step.fadeInMs;
        const uint64_t fullBrightnessEndMs = fadeInEndMs + step.durationMs;
        const uint64_t stepEndMs = fullBrightnessEndMs + step.fadeOutMs;

        if (timeInSequenceMs < stepStartMs || timeInSequenceMs >= stepEndMs)
            continue;

        float intensity = 1.0f;

        if (timeInSequenceMs < fadeInEndMs && step.fadeInMs > 0) {
            intensity = static_cast<float>(timeInSequenceMs - stepStartMs) / static_cast<float>(step.fadeInMs);
        } else if (timeInSequenceMs >= fullBrightnessEndMs && step.fadeOutMs > 0) {
            intensity = static_cast<float>(timeInSequenceMs - fullBrightnessEndMs) / static_cast<float>(step.fadeOutMs);
            intensity = 1.0f - intensity;
        }

        resultColor = step.color.withBrightness(intensity);
    }

    return resultColor;
}

SequenceEffect::SequenceEffect(const std::vector<SequenceStep> &sequence, const absolute_time_t startTime,
                               const bool looping)
    : IRGBEffect(EffectType::SEQUENCE), sequence(sequence), totalDurationMs(0), looping(looping), startTime(startTime) {
    for (const auto &step : sequence) {
        uint64_t stepEndTime = step.startTimeMs + step.durationMs + step.fadeInMs + step.fadeOutMs;
        totalDurationMs = std::max(totalDurationMs, stepEndTime);
    }
}

SequenceEffect::SequenceEffect(InterDeviceCommunicator &communicator)
    : IRGBEffect(EffectType::SEQUENCE), totalDurationMs(0) {
    const uint32_t sequenceSize = communicator.receive32();

    for (uint32_t i = 0; i < sequenceSize; i++) {
        uint8_t id = communicator.receive();
        bool idIsLedId = communicator.receive();
        uint64_t startTimeMs = communicator.receive64();
        Color color(communicator);
        uint64_t durationMs = communicator.receive16();
        uint64_t fadeInMs = communicator.receive16();
        uint64_t fadeOutMs = communicator.receive16();

        sequence.emplace_back(id, idIsLedId, startTimeMs, color, durationMs, fadeInMs, fadeOutMs);
    }

    looping = communicator.receive();

    startTime = communicator.receive64();

    for (const auto &step : sequence) {
        uint64_t stepEndTime = step.startTimeMs + step.durationMs + step.fadeInMs + step.fadeOutMs;
        totalDurationMs = std::max(totalDurationMs, stepEndTime);
    }
}
