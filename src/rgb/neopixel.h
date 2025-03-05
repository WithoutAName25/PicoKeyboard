#pragma once

#include "hardware/pio.h"

void rgb_init(PIO pio, uint pin);

void rgb_put_pixel(uint32_t pixel_grbw);
