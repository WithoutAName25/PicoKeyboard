#ifndef PICOKEYBOARD_RAINBOWEFFECT_H
#define PICOKEYBOARD_RAINBOWEFFECT_H

#include "IRgbEffect.h"
#include "../../util/Task.h"

class RainbowEffect : public IRgbEffect, public IExecutable {
private:
    Task *task;
    RGBController *rgbController;
public:
    void start(RGBController &controller) override;

    void stop() override;

    void execute(absolute_time_t timestamp) override;
};

#endif //PICOKEYBOARD_RAINBOWEFFECT_H
