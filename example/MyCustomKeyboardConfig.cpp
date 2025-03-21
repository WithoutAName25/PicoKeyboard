#include "PicoKeyboard.h"

void configureKeys(KeyActionController &keyActionController) {
#ifdef KEYBOARD_PRIMARY
    KeyLayer &exampleBaseLayer = keyActionController.addLayer();
    keyActionController.switchBaseLayer(exampleBaseLayer);

    exampleBaseLayer.setAction(0, KeyAction(HID_KEY_SPACE));
#else
    (void) keyActionController;
#endif
}

HWKeyConfig hwKeysPrimary[] = {
        HWKeyConfig(0, 0),
        HWKeyConfig(1, 1),
        HWKeyConfig(2, 2),
};

LedConfig ledConfigsPrimary[] = {
        LedConfig(0, 0, 0, 0, 0),
        LedConfig(1, 1, 1, 0, 1),
        LedConfig(2, 2, 0, 1, 2),
        LedConfig(3, 6, -1, -1),
        LedConfig(4, 7, -2, -1)
};

HWKeyConfig hwKeysSecondary[] = {
        HWKeyConfig(0, 3),
        HWKeyConfig(1, 4),
        HWKeyConfig(2, 5),
};

LedConfig ledConfigsSecondary[] = {
        LedConfig(0, 3, 0, 0, 3),
        LedConfig(1, 4, 1, 0, 4),
        LedConfig(2, 5, 0, 1, 5),
        LedConfig(3, 8, -1, -1),
        LedConfig(4, 9, -2, -1)
};

PicoKeyboardConfig getKeyboardConfig() {
    DisplayConfig commonDisplayConfig = DisplayConfig(
            spi1,
            11,
            10,
            9,
            8,
            12,
            25,
            52,
            40,
            135,
            240,
            Degree_90
    );

    return {
            6,
            configureKeys,
            {
                    3,
                    hwKeysPrimary,
                    3,
                    5,
                    ledConfigsPrimary,
                    commonDisplayConfig,
                    uart1,
                    4,
                    5
            },
            {
                    3,
                    hwKeysSecondary,
                    3,
                    5,
                    ledConfigsSecondary,
                    commonDisplayConfig,
                    uart1,
                    4,
                    5
            }
    };
}