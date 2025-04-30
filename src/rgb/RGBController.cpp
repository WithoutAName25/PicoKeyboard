#include "RGBController.h"

#include <algorithm>
#include <communication.h>
#include "neopixel.h"
#include "effect/FadingEffect.h"
#include "effect/IRGBEffect.h"

extern CommandController commandController;
Color black = Color::Black();

void RGBController::setPixel(const uint8_t hwNumber, Color& color) {
    data[hwNumber] = color.toPixelFormat();
}

void RGBController::write() const {
    for (int i = 0; i < numLEDs; ++i) {
        rgb_put_pixel(data[i]);
    }
}

void RGBController::execute(const absolute_time_t timestamp) {
    if (currentEffect != nullptr) currentEffect->update(timestamp);
    for (int i = 0; i < numLEDs; ++i) {
        LedConfig& ledConfig = ledConfigs[i];
        if (currentEffect == nullptr) {
            setPixel(ledConfig.hwNumber, black);
        } else {
            Color color = currentEffect->getColor(ledConfig, timestamp).withBrightness(brightness);
            setPixel(ledConfig.hwNumber, color);
        }
    }
    write();
}

RGBController::RGBController(LedConfig* ledConfigs, LedConfig* mirroredLeds, const uint8_t numLEDs)
    : ledConfigs(ledConfigs), mirroredLeds(mirroredLeds), numLEDs(numLEDs), data(new uint32_t[numLEDs]),
      currentEffect(nullptr) {}

void RGBController::setEffect(absolute_time_t timestamp,
                              const std::shared_ptr<IRGBEffect>& effect,
                              const uint64_t fadeTime,
                              const bool sync) {
    if (sync) {
        commandController.send<RGBEffectCommand>(effect, fadeTime);
    }

    if (currentEffect != nullptr) currentEffect->disable();

    if (fadeTime > 0)
        currentEffect = std::make_shared<FadingEffect>(
            currentEffect,
            effect,
            timestamp,
            timestamp + fadeTime);
    else
        currentEffect = effect;

    if (currentEffect != nullptr) currentEffect->enable(ledConfigs, mirroredLeds, numLEDs);
}

void RGBController::setBrightness(const float brightness, const bool absolute, const bool sync) {
    if (sync) {
        commandController.send<RGBBrightnessCommand>(brightness, absolute);
    }

    this->brightness = std::clamp(absolute ? brightness : this->brightness + brightness, 0.0f, 1.0f);
}
