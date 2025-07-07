#include "KeyAction.h"
#include "usb.h"

extern HIDKeyboard hidKeyboard;

KeyAction::KeyAction(const uint8_t keycode) : keycode(keycode) {}

void KeyAction::press(const absolute_time_t timestamp) { hidKeyboard.pressKey(keycode, timestamp); }

void KeyAction::release(const absolute_time_t timestamp) { hidKeyboard.releaseKey(keycode, timestamp); }