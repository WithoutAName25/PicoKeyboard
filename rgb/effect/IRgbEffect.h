#ifndef PICOKEYBOARD_IRGBEFFECT_H
#define PICOKEYBOARD_IRGBEFFECT_H

#include "../RGBController.h"

class RGBController;

class IRgbEffect {
public:
    virtual ~IRgbEffect() = default;

    virtual void start(RGBController &controller) = 0;

    virtual void stop() {};
};

#endif //PICOKEYBOARD_IRGBEFFECT_H
