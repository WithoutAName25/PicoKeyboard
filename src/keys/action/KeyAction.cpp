#include "KeyAction.h"
#include "usb.h"

extern HIDKeyboard hidKeyboard;

KeyAction::KeyAction(uint8_t keycode) : keycode(keycode) {}

void KeyAction::press() {
    hidKeyboard.pressKey(keycode);
}

void KeyAction::release() {
    hidKeyboard.releaseKey(keycode);
}