#ifndef PICOKEYBOARD_RAINBOWWAVEEFFECT_H
#define PICOKEYBOARD_RAINBOWWAVEEFFECT_H

#include "IRgbEffect.h"
#include "../../util/Task.h"

class RainbowWaveEffect : public IRgbEffect {
public:
    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};

#endif //PICOKEYBOARD_RAINBOWWAVEEFFECT_H
