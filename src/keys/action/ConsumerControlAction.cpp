#include "ConsumerControlAction.h"
#include "usb.h"

extern HIDConsumerControl hidConsumerControl;

ConsumerControlAction::ConsumerControlAction(const uint16_t command) : command(command) {}

void ConsumerControlAction::execute(uint8_t keyId, const KeyState* state, absolute_time_t timestamp) {
    hidConsumerControl.sendCommand(command);
}
