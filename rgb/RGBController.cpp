#include "RGBController.h"
#include "rgb.h"

RGBController::RGBController(uint8_t numPixels)
        : numPixels(numPixels), data(new uint32_t[numPixels]) {
}

RGBController::~RGBController() {
    delete[] data;
}

void RGBController::write() {
    for (int i = 0; i < numPixels; ++i) {
        rgb_put_pixel(data[i]);
    }
}

void RGBController::setPixel(uint8_t index, uint32_t colorGRBW) {
    data[index] = colorGRBW;
}

void RGBController::setAll(uint32_t colorGRBW) {
    for (int i = 0; i < numPixels; ++i) {
        data[i] = colorGRBW;
    }
}
