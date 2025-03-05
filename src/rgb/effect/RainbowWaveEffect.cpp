#include "RainbowWaveEffect.h"
#include "../neopixel.h"

void RainbowWaveEffect::serialize(InterDeviceCommunicator& communicator) {
    IRGBEffect::serialize(communicator);
}

Color RainbowWaveEffect::getColor(LedConfig& led, const absolute_time_t timestamp) {
    return Color::FromHSV(((255 - led.id) * 10 + (timestamp >> 14)) % 360, 1, 1);
}

RainbowWaveEffect::RainbowWaveEffect(): IRGBEffect(EffectType::RAINBOW_WAVE) {}

RainbowWaveEffect::RainbowWaveEffect(InterDeviceCommunicator& communicator): IRGBEffect(EffectType::RAINBOW_WAVE) {}
