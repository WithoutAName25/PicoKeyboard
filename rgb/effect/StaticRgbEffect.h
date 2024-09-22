#ifndef PICOKEYBOARD_STATICRGBEFFECT_H
#define PICOKEYBOARD_STATICRGBEFFECT_H

#include "IRgbEffect.h"

class StaticRgbEffect : public IRgbEffect {
private:
    uint32_t color;
public:
    explicit StaticRgbEffect(uint32_t colorGRBW);

    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};

#endif //PICOKEYBOARD_STATICRGBEFFECT_H
