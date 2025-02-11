#include "LCDLibrary.h"
#include "communication.h"
#include "keys.h"
#include "rgb.h"
#include "rgb_effects.h"
#include "main.h"

#ifdef KEYBOARD_PRIMARY

#include "usb.h"

#endif

#ifdef KEYBOARD_PRIMARY
bool isPrimary = true;
#elif defined(KEYBOARD_SECONDARY)
bool isPrimary = false;
#endif

PicoKeyboardConfig config = getKeyboardConfig();

PicoKeyboardDeviceConfig& deviceConfig = isPrimary ? config.primary : config.secondary;

DisplayConfig& display = deviceConfig.display;

HardwareSPIInterface lcd_spi(display.spiInst, display.dataPin, display.clockPin,
                             display.chipSelectPin, display.dataCommandPin);
LCDDirectGraphics lcd(&lcd_spi, display.resetPin, display.backlightPin,
                      display.hwXOffset,
                      display.hwYOffset, display.hwWidth, display.hwHeight,
                      display.rotation);

Scheduler scheduler;

InterDeviceCommunicator interDeviceCommunicator(deviceConfig.uart, deviceConfig.txPin, deviceConfig.rxPin);

CommandController commandController(interDeviceCommunicator, isPrimary);

RGBController rgbController(deviceConfig.leds, deviceConfig.numLEDs);

#ifdef KEYBOARD_PRIMARY
KeyStateController keyStateController(config.totalNumKeys);
#elif defined(KEYBOARD_SECONDARY)
SecondaryKeyStateController keyStateController(config.totalNumKeys);
#endif

KeyListener keyListener(keyStateController, deviceConfig.hwKeys, deviceConfig.numHwKeys);

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

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0);

    keyListener.setupPins();

    scheduler.addPeriodicTask(&keyListener, get_absolute_time(), 500);
    scheduler.addPeriodicTask(&interDeviceCommunicator, get_absolute_time(), 100);
    scheduler.addPeriodicTask(&commandController, get_absolute_time(), 100);
    scheduler.addPeriodicTask(&rgbController, get_absolute_time(), 5000);

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

    // rgbController.setEffect(KeyStateEffect(RainbowWaveEffect(), 3000));
    RainbowWaveEffect rainbowWaveEffect;
    rgbController.setEffect(&rainbowWaveEffect);
    // rgbController.setEffect(StaticRGBEffect(colorRGB(0, 0, 255)));

    scheduler.run();

    return 0;
}
