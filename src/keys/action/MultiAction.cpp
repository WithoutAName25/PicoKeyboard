#include "MultiAction.h"

void MultiAction::execute(uint8_t keyId, KeyState *state, absolute_time_t timestamp) {
    for (auto &action: actions) {
        action->execute(keyId, state, timestamp);
    }
}
