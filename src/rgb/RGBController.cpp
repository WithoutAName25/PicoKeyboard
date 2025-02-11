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
    for (int i = 0; i < numLEDs; ++i) {
        LedConfig &ledConfig = ledConfigs[i];
        if (currentEffect == nullptr) {
            setPixel(ledConfig.hwNumber, 0);
        } else {
            setPixel(ledConfig.hwNumber, currentEffect->getColor(ledConfig, timestamp));
        }
    }
    write();
}

RGBController::RGBController(LedConfig *ledConfigs, const uint8_t numLEDs)
        : ledConfigs(ledConfigs), numLEDs(numLEDs), data(new uint32_t[numLEDs]), currentEffect(nullptr) {

}

void RGBController::setEffect(IRGBEffect* effect) {
    currentEffect = effect;
}
