#pragma once

#include <pico/stdlib.h>

#include "../communication/InterDeviceCommunicator.h"

class Color {
    // Store both representations
    uint8_t r, g, b;
    uint16_t h;
    float s, v;
    bool rgb_valid;
    bool hsv_valid;
    bool none;

    Color(uint8_t red, uint8_t green, uint8_t blue);
    Color(uint16_t hue, float saturation, float value);

    void updateRGB();
    void updateHSV();

public:
    static Color FromRGB(uint8_t red, uint8_t green, uint8_t blue);
    static Color FromHSV(uint16_t hue, float saturation, float value);

    Color(const Color &other) = default;
    Color &operator=(const Color &other) = default;

    [[nodiscard]] uint8_t red() const;
    [[nodiscard]] uint8_t green() const;
    [[nodiscard]] uint8_t blue() const;
    uint8_t red();
    uint8_t green();
    uint8_t blue();

    [[nodiscard]] uint16_t hue() const;
    [[nodiscard]] float saturation() const;
    [[nodiscard]] float value() const;
    uint16_t hue();
    float saturation();
    float value();

    explicit Color(InterDeviceCommunicator &communicator);
    void serialize(InterDeviceCommunicator &communicator) const;

    uint32_t toPixelFormat();
    uint32_t toPixelFormatW(uint8_t white = 0);

    [[nodiscard]] Color withBrightness(float brightness) const;

    [[nodiscard]] bool isNone() const;

    static Color Black();
    static Color White();
    static Color Red();
    static Color Green();
    static Color Blue();
    static Color Yellow();
    static Color Cyan();
    static Color Magenta();
    static Color Orange();
    static Color Purple();

    static Color None();

    static Color temperature(uint16_t kelvin);
    static Color interpolateHSV(const Color &from, const Color &to, float t);
    static Color interpolateRGB(const Color &from, const Color &to, float t);
    static Color interpolate(const Color &from, const Color &to, float t);
};
