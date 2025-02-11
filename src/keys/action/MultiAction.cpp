#include "MultiAction.h"

void MultiAction::execute(const uint8_t keyId, const KeyState* state, const absolute_time_t timestamp) {
    for (const auto &action: actions) {
        action->execute(keyId, state, timestamp);
    }
}
