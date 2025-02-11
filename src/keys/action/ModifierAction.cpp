#include "ModifierAction.h"
#include "usb.h"

extern HIDKeyboard hidKeyboard;

[[maybe_unused]] ModifierAction::ModifierAction(const Modifier modifier) : modifier(modifier) {}

void ModifierAction::press(absolute_time_t timestamp) {
    hidKeyboard.pressModifier(modifier);
}

void ModifierAction::release(absolute_time_t timestamp) {
    hidKeyboard.releaseModifier(modifier);
}
