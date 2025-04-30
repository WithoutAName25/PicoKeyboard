#include "PerLEDEffect.h"

#include <ranges>

#include "StaticRGBEffect.h"
#include "../RGBController.h"

void PerLEDEffect::serialize(InterDeviceCommunicator& communicator) {
    IRGBEffect::serialize(communicator);
    const uint8_t numEffects = idToEffect.size();
    communicator.send(numEffects);
    for (int i = 0; i < numEffects; ++i) {
        idToEffect[i]->serialize(communicator);
    }
    communicator.send(effectPerLED.size());
    for (const auto& [ledId, effect] : effectPerLED) {
        communicator.send(ledId);
        communicator.send(effectToId.find(effect)->second);
    }
}

Color PerLEDEffect::getColor(LedConfig& led, const absolute_time_t timestamp) {
    if (effectPerLED.contains(led.id)) {
        return effectPerLED[led.id]->getColor(led, timestamp);
    }
    return Color::None();
}

void PerLEDEffect::enable(LedConfig* leds, LedConfig* mirroredLeds, const uint8_t numLEDs) {
    IRGBEffect::enable(leds, mirroredLeds, numLEDs);
    for (auto& effect : effectToId | std::views::keys) {
        effect->enable(leds, mirroredLeds, numLEDs);
    }
}

void PerLEDEffect::disable() {
    IRGBEffect::disable();
}

PerLEDEffect::PerLEDEffect(
    const std::initializer_list<std::pair<const uint8_t, std::shared_ptr<IRGBEffect>>> effectPerLED)
    : IRGBEffect(EffectType::PER_LED), effectPerLED(effectPerLED) {
    uint8_t id = 0;
    for (auto& effect : this->effectPerLED | std::views::values) {
        if (!effectToId.contains(effect)) {
            effectToId.emplace(effect, id);
            idToEffect.emplace(id, effect);
            ++id;
        }
    }
}

PerLEDEffect::PerLEDEffect(InterDeviceCommunicator& communicator) : IRGBEffect(EffectType::PER_LED) {
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
}
