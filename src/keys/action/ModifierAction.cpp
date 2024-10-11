#include "ModifierAction.h"
#include "usb.h"

extern HIDKeyboard hidKeyboard;

ModifierAction::ModifierAction(Modifier modifier) : modifier(modifier) {}

void ModifierAction::press() {
    hidKeyboard.pressModifier(modifier);
}

void ModifierAction::release() {
    hidKeyboard.releaseModifier(modifier);
}
