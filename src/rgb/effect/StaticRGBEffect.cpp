#include "StaticRGBEffect.h"

#include <communication.h>

void StaticRGBEffect::serialize(InterDeviceCommunicator& communicator) {
    IRGBEffect::serialize(communicator);
    communicator.send32(color);
}

uint32_t StaticRGBEffect::getColor(LedConfig& led, absolute_time_t timestamp) {
    return color;
}

StaticRGBEffect::StaticRGBEffect(const uint32_t colorGRBW) : IRGBEffect(EffectType::STATIC_RGB), color(colorGRBW) {}

StaticRGBEffect::StaticRGBEffect(InterDeviceCommunicator& communicator): IRGBEffect(EffectType::STATIC_RGB) {
    color = communicator.receive32();
}
