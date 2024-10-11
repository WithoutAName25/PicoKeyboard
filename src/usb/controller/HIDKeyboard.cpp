#include "HIDKeyboard.h"

void HIDKeyboard::pressModifier(Modifier modifier) {
    modifierPressCounts[(uint) modifier] += 1;
}

void HIDKeyboard::releaseModifier(Modifier modifier) {
    modifierPressCounts[(uint) modifier] -= 1;
}

void HIDKeyboard::pressKey(uint8_t keycode) {
    for (KeyPressState &keyPressState: keyPressStates) {
        if (keyPressState.keycode == keycode) {
            keyPressState.count += 1;
            return;
        }
    }
    keyPressStates.push_back({keycode, 1});
}

void HIDKeyboard::releaseKey(uint8_t keycode) {
    for (auto it = keyPressStates.begin(); it != keyPressStates.end(); ++it) {
        if (it->keycode == keycode) {
            if (--it->count == 0) {
                keyPressStates.erase(it);
            }
            return;
        }
    }
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

    lastReportEmpty = empty;
    return report;
}