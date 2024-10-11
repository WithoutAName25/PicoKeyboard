#include "LCDLibrary.h"
#include "keys.h"
#include "rgb.h"
#include "rgb_effects.h"
#include "rotary_encoder.h"
#include "main.h"

#ifdef KEYBOARD_PRIMARY
#include "key_actions.h"
#include "usb.h"
#endif

#ifdef KEYBOARD_PRIMARY
PicoKeyboardDeviceConfig &deviceConfig = config.primary;
#elifdef KEYBOARD_SECONDARY
PicoKeyboardDeviceConfig &deviceConfig = config.secondary;
#endif

DisplayConfig &display = deviceConfig.display;

HardwareSPIInterface lcd_spi(display.spiInst, display.dataPin, display.clockPin,
                             display.chipSelectPin, display.dataCommandPin);
LCDDirectGraphics lcd(&lcd_spi, display.resetPin, display.backlightPin,
                      display.hwXOffset,
                      display.hwYOffset, display.hwWidth, display.hwHeight,
                      display.rotation);

Scheduler scheduler;

RGBController rgbController(deviceConfig.leds, deviceConfig.numLEDs);

KeyStateController keyStateController(config.totalNumKeys);

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

//void interrupt_callback(uint gpio, uint32_t events) {
//    enc0.callback(gpio);
//    enc1.callback(gpio);
//}

void initEncoder() {
//    irq_set_enabled(IO_IRQ_BANK0, true);
//    gpio_set_irq_callback(interrupt_callback);
//    enc0.init();
//    enc1.init();
}

int main() {
//    stdio_init_all();
    initEncoder();

    rgb_init(pio0, deviceConfig.ledPin);

    lcd.init();
    lcd.setBrightness(0);
    lcd.clear(0);

    keyListener.setupPins();

    scheduler.addPeriodicTask(&rgbController, get_absolute_time(), 5000);
    scheduler.addPeriodicTask(&keyListener, get_absolute_time(), 500);

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

//    rgbController.setEffect(KeyStateEffect(RainbowWaveEffect(), 3000));
    rgbController.setEffect(RainbowWaveEffect());
//    rgbController.setEffect(StaticRGBEffect(colorRGB(0, 0, 255)));

    scheduler.run();

    return 0;
}
