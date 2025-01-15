#include "ConsumerControlAction.h"
#include "usb.h"

extern HIDConsumerControl hidConsumerControl;

ConsumerControlAction::ConsumerControlAction(uint16_t command) : command(command) {}

void ConsumerControlAction::execute(uint8_t keyId, KeyState* state, absolute_time_t timestamp) {
    hidConsumerControl.sendCommand(command);
}
