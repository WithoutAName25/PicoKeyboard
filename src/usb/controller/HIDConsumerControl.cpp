#include "HIDConsumerControl.h"

HIDConsumerControl::HIDConsumerControl() = default;

void HIDConsumerControl::sendCommand(const uint16_t command) { queue.push(command); }

bool HIDConsumerControl::hasReport() { return !queue.empty() || !lastReportEmpty; }

uint16_t HIDConsumerControl::getReport() {
    if (!queue.empty()) {
        const uint16_t command = queue.front();
        queue.pop();
        return command;
    }
    return 0;
}
