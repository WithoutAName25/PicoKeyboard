#include "HIDKeyboard.h"

void HIDKeyboard::pressModifier(Modifier modifier) {
    modifierPressCounts[(uint) modifier] += 1;
}

void HIDKeyboard::releaseModifier(Modifier modifier) {
    modifierPressCounts[(uint) modifier] -= 1;
}

void HIDKeyboard::pressKey(uint8_t keycode, absolute_time_t timestamp) {
    for (KeyPressState &keyPressState: keyPressStates) {
        if (keyPressState.keycode == keycode) {
            keyPressState.count += 1;
            return;
        }
    }
    keyPressStates.emplace_back(keycode, 1, timestamp);
}

void HIDKeyboard::releaseKey(uint8_t keycode, absolute_time_t timestamp) {
    for (auto it = keyPressStates.begin(); it != keyPressStates.end(); ++it) {
        if (it->keycode == keycode) {
            if (--it->count == 0 && it->reported) {
                keyPressStates.erase(it);
            }
            return;
        }
    }
}

KeyBlockingReference HIDKeyboard::blockNewKeys(absolute_time_t timestamp) {
    blockingTimes.push_back(timestamp);
    return --blockingTimes.end();
}

void HIDKeyboard::unblockNewKeys(KeyBlockingReference reference) {
    blockingTimes.erase(reference);
}

bool HIDKeyboard::hasReport() {
    bool hasReport = false;

    for (uint8_t modifierPressCount: modifierPressCounts) {
        if (modifierPressCount > 0) {
            hasReport = true;
            break;
        }
    }

    if (!hasReport) {
        hasReport = !keyPressStates.empty();
    }

    return !lastReportEmpty || hasReport;
}

hid_keyboard_report_t HIDKeyboard::getReport() {
    hid_keyboard_report_t report = {0};
    bool empty = true;

    for (int i = 0; i < 8; ++i) {
        if (modifierPressCounts[i] > 0) {
            report.modifier |= 1 << i;
            empty = false;
        }
    }

    for (int i = 0; i < std::min<size_t>(6, keyPressStates.size()); ++i) {
        report.keycode[i] = keyPressStates.at(i).keycode;
        empty = false;
    }
    int i = 0;
    for (auto state = keyPressStates.begin(); state != keyPressStates.end();) {
        if (state->count == 0 && state->reported) {
            state = keyPressStates.erase(state);
            continue;
        }
        if (blockingTimes.empty() || state->activation < blockingTimes.front()) {
            report.keycode[i] = state->keycode;
            state->reported = true;
            empty = false;
            if (++i >= 6) break;
        }
        ++state;
    }

    lastReportEmpty = empty;
    return report;
}
