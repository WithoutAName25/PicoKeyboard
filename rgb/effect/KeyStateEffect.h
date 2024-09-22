#ifndef PICOKEYBOARD_KEYSTATEEFFECT_H
#define PICOKEYBOARD_KEYSTATEEFFECT_H


#include <memory>
#include "IRgbEffect.h"
#include "../../util/IExecutable.h"
#include "../../util/Task.h"

class KeyStateEffect : public IRgbEffect {
private:
    std::unique_ptr<IRgbEffect> colorEffect;
    uint64_t fadeOutMs;
public:
    explicit KeyStateEffect(std::unique_ptr<IRgbEffect> colorEffect, uint64_t fadeOutMs);

    uint32_t getColor(LedConfig &led, absolute_time_t timestamp) override;
};


#endif //PICOKEYBOARD_KEYSTATEEFFECT_H
