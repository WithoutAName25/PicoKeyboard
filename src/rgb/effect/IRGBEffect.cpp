#include "IRGBEffect.h"

#include <communication.h>
#include "KeyStateEffect.h"
#include "RainbowWaveEffect.h"
#include "StaticRGBEffect.h"
#include "HeatmapEffect.h"

void IRGBEffect::serialize(InterDeviceCommunicator& communicator) {
    communicator.send(static_cast<uint8_t>(type));
}

std::shared_ptr<IRGBEffect> IRGBEffect::create(InterDeviceCommunicator& communicator) {
    switch (static_cast<EffectType>(communicator.receive())) {
    case EffectType::KEY_STATE:
        return std::make_shared<KeyStateEffect>(communicator);
    case EffectType::RAINBOW_WAVE:
        return std::make_shared<RainbowWaveEffect>(communicator);
    case EffectType::STATIC_RGB:
        return std::make_shared<StaticRGBEffect>(communicator);
    case EffectType::HEATMAP:
        return std::make_shared<HeatmapEffect>(communicator);
    }
    return std::make_shared<StaticRGBEffect>(Color::Red());
}