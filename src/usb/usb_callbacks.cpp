/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <algorithm>
#include "tusb.h"
#include "usb_descriptors.h"
#include "controller/HIDCommunication.h"
#include "controller/HIDConsumerControl.h"
#include "controller/HIDGamepad.h"
#include "controller/HIDKeyboard.h"
#include "controller/HIDMouse.h"

extern HIDKeyboard hidKeyboard;
extern HIDMouse hidMouse;
extern HIDConsumerControl hidConsumerControl;
extern HIDGamepad hidGamepad;
extern HIDCommunication hidCommunication;

uint16_t writeToBuffer(const void *report, uint16_t reportLen, void *buffer, uint16_t bufferLen) {
    uint16_t size = std::min(reportLen, bufferLen);
    memcpy(buffer, report, size);
    return size;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
    if (report_type == HID_REPORT_TYPE_INPUT) {
        switch (report_id) {
            case REPORT_ID_KEYBOARD: {
                hid_keyboard_report_t report = hidKeyboard.getReport();
                return writeToBuffer(&report, sizeof(hid_keyboard_report_t), buffer, reqlen);
            }
            case REPORT_ID_MOUSE: {
                hid_mouse_report_t report = hidMouse.getReport();
                return writeToBuffer(&report, sizeof(hid_mouse_report_t), buffer, reqlen);
            }
            case REPORT_ID_CONSUMER_CONTROL: {
                uint16_t report = hidConsumerControl.getReport();
                return writeToBuffer(&report, sizeof(uint16_t), buffer, reqlen);
            }
            case REPORT_ID_GAMEPAD: {
                hid_gamepad_report_t report = hidGamepad.getReport();
                return writeToBuffer(&report, sizeof(hid_gamepad_report_t), buffer, reqlen);
            }
            case REPORT_ID_COMMUNICATION: {
                hid_communication_report_t report = hidCommunication.getReport();
                return writeToBuffer(&report, sizeof(hid_communication_report_t), buffer, reqlen);
            }
            default:
                return 0;
        }
    }
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;

    // TODO
//    if (report_type == HID_REPORT_TYPE_OUTPUT) {
//        // Set keyboard LED e.g Capslock, Numlock etc...
//        if (report_id == REPORT_ID_KEYBOARD) {
//            // bufsize should be (at least) 1
//            if (bufsize < 1) return;
//
//            uint8_t const kbd_leds = buffer[0];
//            (void) kbd_leds;
//        }
//    }
}
