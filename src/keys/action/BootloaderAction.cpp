#include "BootloaderAction.h"

#include "pico/bootrom.h"

void BootloaderAction::execute(uint8_t keyId, const KeyState *state, absolute_time_t timestamp) {
    reset_usb_boot(0, 0);
}
