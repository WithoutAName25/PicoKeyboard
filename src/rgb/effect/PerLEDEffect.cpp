#include "PerLEDEffect.h"

#include <ranges>
#include <utility>

#include "../RGBController.h"
#include "StaticRGBEffect.h"

void PerLEDEffect::serialize(InterDeviceCommunicator &communicator) {
    IRGBEffect::serialize(communicator);
    const uint8_t numEffects = idToEffect.size();
    communicator.send(numEffects);
    for (int i = 0; i < numEffects; ++i) {
        idToEffect[i]->serialize(communicator);
    }

    communicator.send(effectPerLED.size());
    for (const auto &[ledId, effect] : effectPerLED) {
        communicator.send(ledId);
        communicator.send(effectToId.find(effect)->second);
    }

    const bool hasFallback = fallback != nullptr;
    communicator.send(hasFallback);
    if (hasFallback) {
        fallback->serialize(communicator);
    }
}

Color PerLEDEffect::getColor(LedConfig &led, const absolute_time_t timestamp) {
    if (effectPerLED.contains(led.id)) {
        return effectPerLED[led.id]->getColor(led, timestamp);
    }

    if (fallback != nullptr)
        return fallback->getColor(led, timestamp);

    return Color::None();
}

void PerLEDEffect::enable(LedConfig *leds, LedConfig *mirroredLeds, const uint8_t numLEDs) {
    IRGBEffect::enable(leds, mirroredLeds, numLEDs);

    for (auto &effect : effectToId | std::views::keys) {
        effect->enable(leds, mirroredLeds, numLEDs);
    }

    if (fallback != nullptr)
        fallback->enable(leds, mirroredLeds, numLEDs);
}

void PerLEDEffect::disable() {
    IRGBEffect::disable();

    for (auto &effect : effectToId | std::views::keys) {
        effect->disable();
    }

    if (fallback != nullptr)
        fallback->disable();
}

PerLEDEffect::PerLEDEffect(
    const std::initializer_list<std::pair<const uint8_t, std::shared_ptr<IRGBEffect>>> effectPerLED,
    std::shared_ptr<IRGBEffect> fallback)
    : IRGBEffect(EffectType::PER_LED), effectPerLED(effectPerLED), fallback(std::move(fallback)) {
    uint8_t id = 0;
    for (auto &effect : this->effectPerLED | std::views::values) {
        if (!effectToId.contains(effect)) {
            effectToId.emplace(effect, id);
            idToEffect.emplace(id, effect);
            ++id;
        }
    }
}

PerLEDEffect::PerLEDEffect(InterDeviceCommunicator &communicator) : IRGBEffect(EffectType::PER_LED) {
    const uint8_t numEffects = communicator.receive();
    for (int i = 0; i < numEffects; ++i) {
        std::shared_ptr<IRGBEffect> effect = create(communicator);
        idToEffect.emplace(i, effect);
        effectToId.emplace(effect, i);
    }
    const uint8_t numLEDs = communicator.receive();
    for (int i = 0; i < numLEDs; ++i) {
        const uint8_t ledId = communicator.receive();
        const uint8_t effectId = communicator.receive();
        effectPerLED.emplace(ledId, idToEffect[effectId]);
    }

    if (communicator.receive()) {
        fallback = create(communicator);
    }
}
