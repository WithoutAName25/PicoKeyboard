#include "IRGBEffect.h"

#include <communication.h>

#include "FadingEffect.h"
#include "KeyStateEffect.h"
#include "RainbowWaveEffect.h"
#include "StaticRGBEffect.h"
#include "HeatmapEffect.h"
#include "PerLEDEffect.h"
#include "SequenceEffect.h"

void IRGBEffect::serialize(InterDeviceCommunicator& communicator) {
    communicator.send(static_cast<uint8_t>(type));
}

std::shared_ptr<IRGBEffect> IRGBEffect::create(InterDeviceCommunicator& communicator) {
    switch (static_cast<EffectType>(communicator.receive())) {
    case EffectType::FADING:
        return std::make_shared<FadingEffect>(communicator);
    case EffectType::HEATMAP:
        return std::make_shared<HeatmapEffect>(communicator);
    case EffectType::KEY_STATE:
        return std::make_shared<KeyStateEffect>(communicator);
    case EffectType::PER_LED:
        return std::make_shared<PerLEDEffect>(communicator);
    case EffectType::RAINBOW_WAVE:
        return std::make_shared<RainbowWaveEffect>(communicator);
    case EffectType::SEQUENCE:
        return std::make_shared<SequenceEffect>(communicator);
    case EffectType::STATIC_RGB:
        return std::make_shared<StaticRGBEffect>(communicator);
    }
    return std::make_shared<StaticRGBEffect>(Color::Red());
}
