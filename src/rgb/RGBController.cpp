#include "RGBController.h"

#include "effect/FadingEffect.h"
#include "effect/IRGBEffect.h"
#include "neopixel.h"
#include <algorithm>
#include <communication.h>

extern CommandController commandController;
Color black = Color::Black();

void RGBController::setPixel(const uint8_t hwNumber, Color &color) { data[hwNumber] = color.toPixelFormat(); }

void RGBController::write() const {
    for (int i = 0; i < numLEDs; ++i) {
        rgb_put_pixel(data[i]);
    }
}

Color RGBController::getColor(const absolute_time_t timestamp, LedConfig &ledConfig) const {
    for (const auto &overlayEffect : overlayEffects) {
        if (Color color = overlayEffect->getColor(ledConfig, timestamp); !color.isNone())
            return color;
    }

    if (currentEffect == nullptr)
        return black;

    return currentEffect->getColor(ledConfig, timestamp);
}

void RGBController::execute(const absolute_time_t timestamp) {
    if (currentEffect != nullptr)
        currentEffect->update(timestamp);
    for (int i = 0; i < numLEDs; ++i) {
        LedConfig &ledConfig = ledConfigs[i];
        Color color = getColor(timestamp, ledConfig).withBrightness(brightness);
        setPixel(ledConfig.hwNumber, color);
    }
    write();
}

RGBController::RGBController(LedConfig *ledConfigs, LedConfig *mirroredLeds, const uint8_t numLEDs)
    : ledConfigs(ledConfigs), mirroredLeds(mirroredLeds), numLEDs(numLEDs), data(new uint32_t[numLEDs]),
      currentEffect(nullptr) {}

void RGBController::setEffect(absolute_time_t timestamp, const std::shared_ptr<IRGBEffect> &effect,
                              const uint64_t fadeTime, const bool sync) {
    if (sync) {
        commandController.send<RGBEffectCommand>(effect, fadeTime);
    }

    if (currentEffect != nullptr)
        currentEffect->disable();

    if (fadeTime > 0)
        currentEffect = std::make_shared<FadingEffect>(currentEffect, effect, timestamp, timestamp + fadeTime);
    else
        currentEffect = effect;

    if (currentEffect != nullptr)
        currentEffect->enable(ledConfigs, mirroredLeds, numLEDs);
}

void RGBController::setBrightness(const float brightness, const bool absolute, const bool sync) {
    if (sync) {
        commandController.send<RGBBrightnessCommand>(brightness, absolute);
    }

    this->brightness = std::clamp(absolute ? brightness : this->brightness + brightness, 0.0f, 1.0f);
}

OverlayEffectReference RGBController::addOverlayEffect(const std::shared_ptr<IRGBEffect> &effect, const bool sync) {
    if (sync) {
        commandController.send<RGBOverlayEffectCommand>(effect);
    }

    overlayEffects.emplace_front(effect);
    return overlayEffects.begin();
}

void RGBController::removeOverlayEffect(const OverlayEffectReference reference) {
    const auto index = static_cast<uint16_t>(std::distance(overlayEffects.begin(), reference));
    commandController.send<RGBOverlayEffectCommand>(index);

    overlayEffects.erase(reference);
}

void RGBController::removeOverlayEffect(const uint16_t id) {
    auto it = overlayEffects.begin();
    std::advance(it, id);
    if (it != overlayEffects.end()) {
        overlayEffects.erase(it);
    }
}
