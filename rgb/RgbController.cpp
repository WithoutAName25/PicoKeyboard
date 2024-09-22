#include "RgbController.h"
#include "rgb.h"
#include "effect/IRgbEffect.h"

void RgbController::setPixel(uint8_t hwNumber, uint32_t colorGRBW) {
    data[hwNumber] = colorGRBW;
}

void RgbController::write() {
    for (int i = 0; i < numLEDs; ++i) {
        rgb_put_pixel(data[i]);
    }
}

void RgbController::execute(absolute_time_t timestamp) {
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

RgbController::RgbController(Scheduler &scheduler, LedConfig *ledConfigs, uint8_t numLEDs)
        : ledConfigs(ledConfigs), numLEDs(numLEDs), data(new uint32_t[numLEDs]), currentEffect(nullptr) {
    scheduler.addPeriodicTask(this, get_absolute_time(), 5000);
}

void RgbController::setEffect(std::unique_ptr<IRgbEffect> effect) {
    currentEffect = std::move(effect);
}
