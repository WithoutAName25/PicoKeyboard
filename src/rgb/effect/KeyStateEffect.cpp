#include "KeyStateEffect.h"

#include "../neopixel.h"
#include "keys.h"
#include <communication.h>

extern KeyStateController keyStateController;

void KeyStateEffect::serialize(InterDeviceCommunicator &communicator) {
    IRGBEffect::serialize(communicator);
    communicator.send64(fadeOutMs);
    communicator.send(mirrored);
    inactiveEffect->serialize(communicator);
    activeEffect->serialize(communicator);
}

uint64_t KeyStateEffect::getTimeSinceRelease(const uint8_t keyId, const absolute_time_t timestamp) {
    const KeyState &state = keyStateController.getKeyState(keyId);

    if (state.isPressed)
        return 0;

    if (state.totalPressCount == 0)
        return -1;

    return timestamp - state.releaseTime;
}

Color KeyStateEffect::getColor(LedConfig &led, const absolute_time_t timestamp) {
    if (!led.isKeyLed)
        return Color::None();

    uint64_t sinceRelease = getTimeSinceRelease(led.associatedKeyId, timestamp);
    if (mirrored) {
        if (const auto mirroredKey = mirroredKeyMap.find(led.associatedKeyId); mirroredKey != mirroredKeyMap.end()) {
            sinceRelease = std::min(sinceRelease, getTimeSinceRelease(mirroredKey->second, timestamp));
        }
    }

    if (sinceRelease >= fadeOutMs * 1000)
        return inactiveEffect->getColor(led, timestamp);

    const float fadeAmount = static_cast<float>(sinceRelease) / static_cast<float>(fadeOutMs * 1000);
    const Color inactive = inactiveEffect->getColor(led, timestamp);
    const Color active = activeEffect->getColor(led, timestamp);
    return Color::interpolate(active, inactive, fadeAmount);
}

void KeyStateEffect::enable(LedConfig *leds, LedConfig *mirroredLeds, const uint8_t numLEDs) {
    IRGBEffect::enable(leds, mirroredLeds, numLEDs);
    if (mirrored && mirroredLeds) {
        mirroredKeyMap.clear();
        for (uint8_t i = 0; i < numLEDs; i++) {
            if (leds[i].isKeyLed && mirroredLeds[i].isKeyLed) {
                mirroredKeyMap[leds[i].associatedKeyId] = mirroredLeds[i].associatedKeyId;
            }
        }
    }
}

KeyStateEffect::KeyStateEffect(InterDeviceCommunicator &communicator) : IRGBEffect(EffectType::KEY_STATE) {
    fadeOutMs = communicator.receive64();
    mirrored = communicator.receive();
    inactiveEffect = create(communicator);
    activeEffect = create(communicator);
}
