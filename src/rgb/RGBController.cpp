#include "RGBController.h"
#include "neopixel.h"
#include "effect/IRGBEffect.h"

void RGBController::setPixel(const uint8_t hwNumber, const uint32_t colorGRBW) {
    data[hwNumber] = colorGRBW;
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
            setPixel(ledConfig.hwNumber, 0);
        } else {
            setPixel(ledConfig.hwNumber, currentEffect->getColor(ledConfig, timestamp));
        }
    }
    write();
}

RGBController::RGBController(LedConfig* ledConfigs, LedConfig* mirroredLeds, const uint8_t numLEDs)
    : ledConfigs(ledConfigs), mirroredLeds(mirroredLeds), numLEDs(numLEDs), data(new uint32_t[numLEDs]),
      currentEffect(nullptr) {}

void RGBController::setEffect(const std::shared_ptr<IRGBEffect>& effect) {
    if (currentEffect != nullptr) currentEffect->disable();
    currentEffect = effect;
    if (currentEffect != nullptr) currentEffect->enable(ledConfigs, mirroredLeds, numLEDs);
}
