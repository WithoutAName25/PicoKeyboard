#include "neopixel.h"
#include "neopixel.pio.h"
#include "hardware/clocks.h"

auto currentPio = pio0;
uint currentSM = 0;

void rgb_init(PIO pio, const uint pin) {
    const uint sm = pio_claim_unused_sm(pio, true);
    const uint offset = pio_add_program(pio, &rgb_program);

    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    pio_sm_config c = rgb_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    constexpr int cycles_per_bit = rgb_T1 + rgb_T2 + rgb_T3;
    const float div = static_cast<float>(clock_get_hz(clk_sys)) / (800000.0f * static_cast<float>(cycles_per_bit));
    sm_config_set_clkdiv(&c, div);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);

    currentPio = pio;
    currentSM = sm;
}

void rgb_put_pixel(const uint32_t pixel_grbw) {
    pio_sm_put_blocking(currentPio, currentSM, pixel_grbw);
}
