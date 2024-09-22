#ifndef PICOKEYBOARD_IRGBEFFECT_H
#define PICOKEYBOARD_IRGBEFFECT_H

#include "../LedConfig.h"

class IRgbEffect {
public:
    virtual ~IRgbEffect() = default;

    virtual uint32_t getColor(LedConfig &led, absolute_time_t timestamp) = 0;
};

#endif //PICOKEYBOARD_IRGBEFFECT_H
