#include "main.h"
#include "LCDLibrary.h"
#include "communication.h"
#include "keys.h"
#include "rgb.h"
#include "rgb_effects.h"

#include <valarray>

#include "util/LambdaExecutable.h"

#ifdef KEYBOARD_PRIMARY

#include "usb.h"

#endif

#ifdef KEYBOARD_PRIMARY
bool isPrimary = true;
#elif defined(KEYBOARD_SECONDARY)
bool isPrimary = false;
#endif

PicoKeyboardConfig config = getKeyboardConfig();

PicoKeyboardDeviceConfig &deviceConfig = *(isPrimary ? config.primary : config.secondary);
PicoKeyboardDeviceConfig &otherDeviceConfig = *(isPrimary ? config.secondary : config.primary);

Scheduler scheduler;

InterDeviceCommunicator interDeviceCommunicator(deviceConfig.uart, deviceConfig.txPin, deviceConfig.rxPin);

CommandController commandController(interDeviceCommunicator, isPrimary);

RGBController rgbController(deviceConfig.leds, config.isMirrored ? otherDeviceConfig.leds : nullptr,
                            deviceConfig.numLEDs);

KeyStateController keyStateController(config.totalNumKeys);

IKeyListener *keyListener;

#ifdef KEYBOARD_PRIMARY
KeyActionController keyActionController(config.totalNumKeys, keyStateController);

HIDKeyboard hidKeyboard;
HIDMouse hidMouse;
HIDConsumerControl hidConsumerControl;
HIDGamepad hidGamepad;
HIDCommunication hidCommunication;

TUDTask tudTask;
#endif

// void interrupt_callback(uint gpio, uint32_t events) {
//     enc0.callback(gpio);
//     enc1.callback(gpio);
// }

void initEncoder() {
    // irq_set_enabled(IO_IRQ_BANK0, true);
    // gpio_set_irq_callback(interrupt_callback);
    // enc0.init();
    // enc1.init();
}

int main() {
    // stdio_init_all();
    initEncoder();

    rgb_init(pio0, deviceConfig.ledPin);

    if (deviceConfig.display != nullptr) {
        const DisplayConfig &display = *deviceConfig.display;

        HardwareSPIInterface lcd_spi(display.spiInst, display.dataPin, display.clockPin, display.chipSelectPin,
                                     display.dataCommandPin);
        LCDDirectGraphics lcd(&lcd_spi, display.resetPin, display.backlightPin, display.hwXOffset, display.hwYOffset,
                              display.hwWidth, display.hwHeight, display.rotation);
        lcd.init();
        lcd.setBrightness(0);
        lcd.clear(0);
    }

    if (deviceConfig.hwKeys != nullptr) {
        keyListener = new KeyListener(keyStateController, deviceConfig.numHwKeys, deviceConfig.hwKeys);
    } else {
        keyListener = new KeyMatrixListener(keyStateController, deviceConfig.numHwKeys, deviceConfig.hwMatrixKeys);
    }

    keyListener->setupPins();

    scheduler.addPeriodicTask(keyListener, get_absolute_time(), 500);
    scheduler.addPeriodicTask(&interDeviceCommunicator, get_absolute_time(), 100);
    scheduler.addPeriodicTask(&commandController, get_absolute_time(), 100);
    scheduler.addPeriodicTask(&rgbController, get_absolute_time(), 10000);

#ifdef KEYBOARD_PRIMARY
    tud_init(BOARD_TUD_RHPORT);

    config.configureKeys(keyActionController);

    scheduler.addPeriodicTask(&tudTask, get_absolute_time(), 500);

    scheduler.addPeriodicTask(&hidKeyboard, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&hidMouse, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&hidConsumerControl, get_absolute_time(), 10000);
    scheduler.addPeriodicTask(&hidGamepad, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&hidCommunication, get_absolute_time(), 10000);
#endif

    config.configureStartup(
        [](const absolute_time_t time, const std::function<void(absolute_time_t timestamp)> &block) {
            // ReSharper disable once CppUseAuto
            LambdaExecutable *task = // NOLINT(*-use-auto)
                new LambdaExecutable([block, task](const absolute_time_t timestamp) {
                    block(timestamp);
                    delete task;
                });
            scheduler.addTask(task, time);
        });

    scheduler.run();

    return 0;
}
