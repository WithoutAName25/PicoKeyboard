#include "KeyStateEffect.h"
#include "keys.h"
#include "../neopixel.h"

extern KeyStateController keyStateController;

uint32_t KeyStateEffect::getColor(LedConfig &led, const absolute_time_t timestamp) {
    if (!led.isKeyLed) return 0;

    const KeyState &state = keyStateController.getKeyState(led.associatedKeyId);
    uint64_t sinceRelease = timestamp - state.releaseTime;

    if (state.isPressed) sinceRelease = 0;
    else if (state.totalPressCount == 0 || sinceRelease >= fadeOutMs * 1000) return 0;

    const float fadeAmount = static_cast<float>(sinceRelease) / static_cast<float>(fadeOutMs * 1000);
    return brightness(colorEffect->getColor(led, timestamp), 1 - fadeAmount);
}
