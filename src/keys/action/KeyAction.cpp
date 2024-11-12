#include "KeyAction.h"
#include "usb.h"

extern HIDKeyboard hidKeyboard;

KeyAction::KeyAction(uint8_t keycode) : keycode(keycode) {}

void KeyAction::press(absolute_time_t timestamp) {
    hidKeyboard.pressKey(keycode, timestamp);
}

void KeyAction::release(absolute_time_t timestamp) {
    hidKeyboard.releaseKey(keycode, timestamp);
}