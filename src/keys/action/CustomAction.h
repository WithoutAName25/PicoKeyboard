#pragma once
#include "PressReleaseAction.h"

class CustomAction final : public PressReleaseAction {
    std::function<void(absolute_time_t timestamp)> onPress;
    std::function<void(absolute_time_t timestamp)> onRelease;

    void press(absolute_time_t timestamp) override;
    void release(absolute_time_t timestamp) override;

public:
    explicit CustomAction(
        const std::function<void(absolute_time_t timestamp)> &onPress = [](absolute_time_t) {},
        const std::function<void(absolute_time_t timestamp)> &onRelease = [](absolute_time_t) {});
};
