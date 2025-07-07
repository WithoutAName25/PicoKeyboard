#include "FadingEffect.h"

void FadingEffect::serialize(InterDeviceCommunicator &communicator) {
    IRGBEffect::serialize(communicator);
    const bool hasOldEffect = oldEffect != nullptr;
    communicator.send(hasOldEffect);
    if (hasOldEffect) {
        oldEffect->serialize(communicator);
    }
    newEffect->serialize(communicator);
    communicator.send64(startTime);
    communicator.send64(endTime);
}

void FadingEffect::enable(LedConfig *leds, LedConfig *mirroredLeds, const uint8_t numLEDs) {
    IRGBEffect::enable(leds, mirroredLeds, numLEDs);
    if (oldEffect != nullptr) {
        oldEffect->enable(leds, mirroredLeds, numLEDs);
    }
    newEffect->enable(leds, mirroredLeds, numLEDs);
}

void FadingEffect::disable() {
    IRGBEffect::disable();
    if (oldEffect != nullptr) {
        oldEffect->disable();
    }
    newEffect->disable();
}

void FadingEffect::update(const absolute_time_t timestamp) {
    IRGBEffect::update(timestamp);
    if (timestamp > endTime && oldEffect != nullptr) {
        oldEffect->disable();
        oldEffect = nullptr;
    }
    if (oldEffect != nullptr) {
        oldEffect->update(timestamp);
    }
    newEffect->update(timestamp);
}

Color FadingEffect::getColor(LedConfig &led, const absolute_time_t timestamp) {
    if (timestamp > endTime) {
        return newEffect->getColor(led, timestamp);
    }
    if (timestamp < startTime) {
        if (oldEffect == nullptr) {
            return Color::None();
        }
        return oldEffect->getColor(led, timestamp);
    }
    const Color oldColor = oldEffect != nullptr ? oldEffect->getColor(led, startTime) : Color::None();
    const Color newColor = newEffect->getColor(led, endTime);
    return Color::interpolate(oldColor, newColor,
                              static_cast<float>(timestamp - startTime) / static_cast<float>(endTime - startTime));
}

FadingEffect::FadingEffect(const std::shared_ptr<IRGBEffect> &oldEffect, const std::shared_ptr<IRGBEffect> &newEffect,
                           const absolute_time_t startTime, const absolute_time_t endTime)
    : IRGBEffect(EffectType::FADING), oldEffect(oldEffect), newEffect(newEffect), startTime(startTime),
      endTime(endTime) {}

FadingEffect::FadingEffect(InterDeviceCommunicator &communicator) : IRGBEffect(EffectType::FADING) {
    if (communicator.receive()) {
        oldEffect = create(communicator);
    }
    newEffect = create(communicator);
    startTime = communicator.receive64();
    endTime = communicator.receive64();
}
