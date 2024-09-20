#include "RainbowEffect.h"
#include "../rgb.h"
#include "../../main.h"

void RainbowEffect::start(RGBController &controller) {
    task = scheduler.addPeriodicTask(this, get_absolute_time(), 5000);
    rgbController = &controller;
}

void RainbowEffect::stop() {
    task->cancel();
}

void RainbowEffect::execute(absolute_time_t timestamp) {
    for (int i = 0; i < 12; ++i) {
        rgbController->setPixel(i, colorHSV(((24 - i) * 10 + (timestamp >> 15)) % 360, 1, 1));
    }
    rgbController->write();
}
