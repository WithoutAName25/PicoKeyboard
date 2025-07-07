#include "CustomAction.h"

void CustomAction::press(const absolute_time_t timestamp) { onPress(timestamp); }

void CustomAction::release(const absolute_time_t timestamp) { onRelease(timestamp); }

CustomAction::CustomAction(const std::function<void(absolute_time_t timestamp)> &onPress,
                           const std::function<void(absolute_time_t timestamp)> &onRelease)
    : onPress(onPress), onRelease(onRelease) {}
