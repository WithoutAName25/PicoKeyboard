#include "HeatmapEffect.h"

#include <cmath>
#include <communication.h>
#include <keys.h>
#include <util.h>

extern KeyStateController keyStateController;
extern Scheduler scheduler;

void HeatmapEffect::serialize(InterDeviceCommunicator& communicator) {
    IRGBEffect::serialize(communicator);
    hot.serialize(communicator);
    cold.serialize(communicator);
    communicator.send32(*reinterpret_cast<uint32_t*>(&coolingFactor));
    communicator.send32(heatingOnPress);
    communicator.send32(heatingRateOnHold);
    communicator.send(mirrored);
}


Color HeatmapEffect::getColor(LedConfig& led, const absolute_time_t timestamp) {
    const uint64_t maxValue = std::max(static_cast<uint64_t>(maxHeat),
                                       static_cast<uint64_t>(heatingOnPress + heatingRateOnHold * 100 + 1));
    const uint64_t heat = heatMap[inverseIdMap[led.id]];
    if (heat == 0) return cold;
    if (heat >= maxValue) return hot;

    const float ratio = static_cast<float>(heat) / static_cast<float>(maxValue);
    return Color::interpolate(cold, hot, ratio);
}

void HeatmapEffect::enable(LedConfig* leds, LedConfig* mirroredLeds, const uint8_t numLEDs) {
    IRGBEffect::enable(leds, mirroredLeds, numLEDs);

    this->leds = leds;
    this->mirroredLeds = mirroredLeds;
    this->numLEDs = numLEDs;

    inverseIdMap.clear();
    for (uint8_t i = 0; i < numLEDs; i++) {
        if (leds[i].isKeyLed) {
            inverseIdMap[leds[i].id] = i;
        }
    }

    heatMap = std::make_unique<uint64_t[]>(numLEDs);

    if (task != nullptr) {
        task->cancel();
        task = nullptr;
    }
    lastTick = get_absolute_time();
    task = scheduler.addPeriodicTask(this, lastTick, 10000);
}

void HeatmapEffect::disable() {
    IRGBEffect::disable();
    if (task != nullptr) {
        task->cancel();
        task = nullptr;
    }
}

void HeatmapEffect::execute(const absolute_time_t timestamp) {
    uint64_t currentMaxHeat = 0;

    for (int i = 0; i < numLEDs; ++i) {
        uint64_t heatDelta = 0;
        if (leds[i].isKeyLed) {
            heatDelta += getHeatDelta(leds[i].associatedKeyId);
        }
        if (mirrored && mirroredLeds != nullptr && mirroredLeds[i].isKeyLed) {
            heatDelta += getHeatDelta(mirroredLeds[i].associatedKeyId);
        }
        heatMap[i] += heatDelta;
        heatMap[i] = static_cast<uint64_t>(static_cast<float>(heatMap[i]) * (1.0f - coolingFactor));
        currentMaxHeat = std::max(currentMaxHeat, heatMap[i]);
    }


    maxHeat = std::max(
        static_cast<float>(currentMaxHeat),
        (maxHeat * (1 - coolingFactor * 0.5f)) + (static_cast<float>(currentMaxHeat) * coolingFactor * 0.5f));
    lastTick = timestamp;
}

uint64_t HeatmapEffect::getHeatDelta(const uint8_t keyId) const {
    uint64_t delta = 0;
    if (keyStateController.getKeyState(keyId).pressTime > lastTick) {
        delta += heatingOnPress;
    }
    if (keyStateController.getKeyState(keyId).isPressed) {
        delta += heatingRateOnHold;
    }
    return delta;
}

HeatmapEffect::HeatmapEffect(
    const Color& hot, const Color& cold,
    const float coolingFactor, const uint32_t heatingOnPress, const uint32_t heatingRateOnHold,
    const bool mirrored)
    : IRGBEffect(EffectType::HEATMAP),
      hot(hot), cold(cold),
      coolingFactor(coolingFactor), heatingOnPress(heatingOnPress), heatingRateOnHold(heatingRateOnHold),
      mirrored(mirrored) {}

HeatmapEffect::HeatmapEffect(InterDeviceCommunicator& communicator)
    : IRGBEffect(EffectType::HEATMAP),
      hot(Color(communicator)), cold(Color(communicator)) {
    uint32_t coolingFactorRaw = communicator.receive32();
    coolingFactor = *reinterpret_cast<float*>(&coolingFactorRaw);
    heatingOnPress = communicator.receive32();
    heatingRateOnHold = communicator.receive32();
    mirrored = communicator.receive();
}
