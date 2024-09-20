#ifndef PICOKEYBOARD_STATICRGBEFFECT_H
#define PICOKEYBOARD_STATICRGBEFFECT_H

#include "IRgbEffect.h"

class StaticRgbEffect : public IRgbEffect {
private:
    uint32_t color;
public:
    StaticRgbEffect(uint32_t colorGRBW);

    void start(RGBController &controller) override;
};

#endif //PICOKEYBOARD_STATICRGBEFFECT_H
