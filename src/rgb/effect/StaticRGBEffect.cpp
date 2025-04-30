#include "StaticRGBEffect.h"

#include <communication.h>

void StaticRGBEffect::serialize(InterDeviceCommunicator& communicator) {
    IRGBEffect::serialize(communicator);
    color.serialize(communicator);
}

Color StaticRGBEffect::getColor(LedConfig& led, absolute_time_t timestamp) {
    return color;
}

StaticRGBEffect::StaticRGBEffect(const Color& color) : IRGBEffect(EffectType::STATIC_RGB), color(color) {}

StaticRGBEffect::StaticRGBEffect(InterDeviceCommunicator& communicator)
    : IRGBEffect(EffectType::STATIC_RGB), color(Color(communicator)) {}
