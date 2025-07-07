#include "PicoKeyboard.h"

#define NUM_KEYS 6

void configureKeys(KeyActionController &keyActionController) {
#ifdef KEYBOARD_PRIMARY
    BasicKeyLayer &text = *new BasicKeyLayer(NUM_KEYS);

    keyActionController.switchBaseLayer(text);

    text.setAction(0, KeyAction(HID_KEY_SPACE));
#else
    (void)keyActionController;
#endif
}

void configureStartup(
    const std::function<void(absolute_time_t time, std::function<void(absolute_time_t timestamp)> block)> &exec) {
    exec(100000, [](const absolute_time_t timestamp) {
        rgbController.setEffect(
            timestamp, std::make_shared<HeatmapEffect>(Color::Red(), Color::Blue(), 0.002, 2000, 30, false), 2500000);
    });
}

PicoKeyboardConfig getKeyboardConfig() {
    auto *hwMatrixKeys = new HWMatrixKeyConfig[NUM_KEYS]{
        HWMatrixKeyConfig(0, 1, 3), HWMatrixKeyConfig(1, 1, 4), HWMatrixKeyConfig(2, 1, 5),
        HWMatrixKeyConfig(3, 2, 3), HWMatrixKeyConfig(4, 2, 4), HWMatrixKeyConfig(5, 2, 5),
    };

    auto *ledConfigs = new LedConfig[8]{LedConfig(0, 0, 0, 0, 0), LedConfig(1, 1, 1, 0, 1), LedConfig(2, 3, 2, 0, 2),
                                        LedConfig(3, 4, 0, 1, 3), LedConfig(4, 5, 1, 1, 4), LedConfig(5, 6, 2, 1, 5),
                                        LedConfig(6, 7, -1, -1),  LedConfig(7, 8, -2, -1)};

    return {NUM_KEYS, configureKeys, configureStartup,
            new PicoKeyboardDeviceConfig(NUM_KEYS, hwMatrixKeys, 6, 8, ledConfigs)};
}
