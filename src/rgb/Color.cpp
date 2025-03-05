#include "Color.h"

#include <algorithm>
#include <cmath>

Color::Color(const uint8_t red, const uint8_t green, const uint8_t blue)
    : r(red), g(green), b(blue),
      h(0), s(0), v(0),
      rgb_valid(true), hsv_valid(false) {}

Color::Color(const uint16_t hue, const float saturation, const float value)
    : r(0), g(0), b(0),
      h(hue % 360),
      s(std::clamp(saturation, 0.0f, 1.0f)),
      v(std::clamp(value, 0.0f, 1.0f)),
      rgb_valid(false), hsv_valid(true) {}

Color Color::FromRGB(uint8_t red, uint8_t green, uint8_t blue) {
    return {red, green, blue};
}

Color Color::FromHSV(uint16_t hue, float saturation, float value) {
    return {hue, saturation, value};
}

uint8_t Color::red() const { return const_cast<Color*>(this)->red(); }
uint8_t Color::green() const { return const_cast<Color*>(this)->green(); }
uint8_t Color::blue() const { return const_cast<Color*>(this)->blue(); }

uint8_t Color::red() {
    if (!rgb_valid) updateRGB();
    return r;
}

uint8_t Color::green() {
    if (!rgb_valid) updateRGB();
    return g;
}

uint8_t Color::blue() {
    if (!rgb_valid) updateRGB();
    return b;
}

uint16_t Color::hue() const { return const_cast<Color*>(this)->hue(); }
float Color::saturation() const { return const_cast<Color*>(this)->saturation(); }
float Color::value() const { return const_cast<Color*>(this)->value(); }

uint16_t Color::hue() {
    if (!hsv_valid) updateHSV();
    return h;
}

float Color::saturation() {
    if (!hsv_valid) updateHSV();
    return s;
}

float Color::value() {
    if (!hsv_valid) updateHSV();
    return v;
}

void Color::setRGB(const uint8_t red, const uint8_t green, const uint8_t blue) {
    r = red;
    g = green;
    b = blue;
    rgb_valid = true;
    hsv_valid = false;
}

void Color::setHSV(const uint16_t hue, const float saturation, const float value) {
    h = hue % 360;
    s = std::clamp(saturation, 0.0f, 1.0f);
    v = std::clamp(value, 0.0f, 1.0f);
    hsv_valid = true;
    rgb_valid = false;
}

Color::Color(InterDeviceCommunicator communicator) {
    r = communicator.receive();
    g = communicator.receive();
    b = communicator.receive();
    h = 0;
    s = 0;
    v = 0;
    rgb_valid = true;
    hsv_valid = false;
}

void Color::serialize(InterDeviceCommunicator communicator) const {
    communicator.send(r);
    communicator.send(g);
    communicator.send(b);
}

uint32_t Color::toPixelFormat() {
    if (!rgb_valid) updateRGB();
    return (static_cast<uint32_t>(g) << 24) |
        (static_cast<uint32_t>(r) << 16) |
        (static_cast<uint32_t>(b) << 8);
}

uint32_t Color::toPixelFormatW(const uint8_t white) {
    if (!rgb_valid) updateRGB();
    return (static_cast<uint32_t>(g) << 24) |
        (static_cast<uint32_t>(r) << 16) |
        (static_cast<uint32_t>(b) << 8) |
        white;
}

Color Color::withBrightness(const float brightness) const {
    const float b = std::clamp(brightness, 0.0f, 1.0f);

    if (hsv_valid) {
        return {h, s, v * b};
    } else {
        return {
            static_cast<uint8_t>(r * b),
            static_cast<uint8_t>(g * b),
            static_cast<uint8_t>(b * b)
        };
    }
}

Color Color::Black() { return FromRGB(0, 0, 0); }
Color Color::White() { return FromRGB(255, 255, 255); }
Color Color::Red() { return FromRGB(255, 0, 0); }
Color Color::Green() { return FromRGB(0, 255, 0); }
Color Color::Blue() { return FromRGB(0, 0, 255); }
Color Color::Yellow() { return FromRGB(255, 255, 0); }
Color Color::Cyan() { return FromRGB(0, 255, 255); }
Color Color::Magenta() { return FromRGB(255, 0, 255); }
Color Color::Orange() { return FromRGB(255, 165, 0); }
Color Color::Purple() { return FromRGB(128, 0, 128); }

Color Color::temperature(uint16_t kelvin) {
    // Approximation of color temperature
    kelvin = std::clamp(kelvin, uint16_t(1000), uint16_t(40000));

    const float temp = kelvin / 100.0f;

    float r, g, b;

    if (temp <= 66) {
        r = 255;
    } else {
        r = temp - 60;
        r = 329.698727446f * std::pow(r, -0.1332047592f);
        r = std::clamp(r, 0.0f, 255.0f);
    }

    if (temp <= 66) {
        g = temp;
        g = 99.4708025861f * std::log(g) - 161.1195681661f;
    } else {
        g = temp - 60;
        g = 288.1221695283f * std::pow(g, -0.0755148492f);
    }
    g = std::clamp(g, 0.0f, 255.0f);

    if (temp >= 66) {
        b = 255;
    } else if (temp <= 19) {
        b = 0;
    } else {
        b = temp - 10;
        b = 138.5177312231f * std::log(b) - 305.0447927307f;
        b = std::clamp(b, 0.0f, 255.0f);
    }

    return {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
}

Color Color::interpolateHSV(const Color& from, const Color& to, float t) {
    t = std::clamp(t, 0.0f, 1.0f);

    const uint16_t h1 = from.hue();
    const uint16_t h2 = to.hue();
    const float s1 = from.saturation();
    const float s2 = to.saturation();
    const float v1 = from.value();
    const float v2 = to.value();

    float h_diff = static_cast<float>(h2) - static_cast<float>(h1);

    if (h_diff > 180) {
        h_diff -= 360;
    } else if (h_diff < -180) {
        h_diff += 360;
    }

    const uint16_t h = static_cast<uint16_t>(static_cast<float>(h1) + t * h_diff + 360) % 360;
    const float s = s1 + t * (s2 - s1);
    const float v = v1 + t * (v2 - v1);

    return {h, s, v};
}

Color Color::interpolateRGB(const Color& from, const Color& to, float t) {
    t = std::clamp(t, 0.0f, 1.0f);

    const uint8_t r1 = from.red();
    const uint8_t g1 = from.green();
    const uint8_t b1 = from.blue();
    const uint8_t r2 = to.red();
    const uint8_t g2 = to.green();
    const uint8_t b2 = to.blue();

    const auto r = static_cast<uint8_t>(r1 + t * (r2 - r1));
    const auto g = static_cast<uint8_t>(g1 + t * (g2 - g1));
    const auto b = static_cast<uint8_t>(b1 + t * (b2 - b1));

    return {r, g, b};
}

Color Color::interpolate(const Color& from, const Color& to, const float t) {
    return interpolateHSV(from, to, t);
}

void Color::updateRGB() {
    if (s <= 0.0f) {
        // Achromatic (grey)
        r = g = b = static_cast<uint8_t>(v * 255);
        rgb_valid = true;
        return;
    }

    const float hh = h / 60.0f;
    const int i = static_cast<int>(hh);
    const float ff = hh - i;
    const float p = v * (1.0f - s);
    const float q = v * (1.0f - (s * ff));
    const float t = v * (1.0f - (s * (1.0f - ff)));

    switch (i) {
    case 0:
        r = static_cast<uint8_t>(v * 255);
        g = static_cast<uint8_t>(t * 255);
        b = static_cast<uint8_t>(p * 255);
        break;
    case 1:
        r = static_cast<uint8_t>(q * 255);
        g = static_cast<uint8_t>(v * 255);
        b = static_cast<uint8_t>(p * 255);
        break;
    case 2:
        r = static_cast<uint8_t>(p * 255);
        g = static_cast<uint8_t>(v * 255);
        b = static_cast<uint8_t>(t * 255);
        break;
    case 3:
        r = static_cast<uint8_t>(p * 255);
        g = static_cast<uint8_t>(q * 255);
        b = static_cast<uint8_t>(v * 255);
        break;
    case 4:
        r = static_cast<uint8_t>(t * 255);
        g = static_cast<uint8_t>(p * 255);
        b = static_cast<uint8_t>(v * 255);
        break;
    default: // case 5
        r = static_cast<uint8_t>(v * 255);
        g = static_cast<uint8_t>(p * 255);
        b = static_cast<uint8_t>(q * 255);
        break;
    }

    rgb_valid = true;
}

void Color::updateHSV() {
    const uint8_t min_val = std::min(std::min(r, g), b);
    const uint8_t max_val = std::max(std::max(r, g), b);
    const uint8_t delta = max_val - min_val;

    v = static_cast<float>(max_val) / 255.0f;

    if (max_val == 0) {
        s = 0.0f;
    } else {
        s = static_cast<float>(delta) / max_val;
    }

    if (delta == 0) {
        h = 0; // Achromatic (no hue)
    } else {
        float hue;

        if (r == max_val) {
            // Between yellow and magenta
            hue = static_cast<float>(g - b) / delta;
        } else if (g == max_val) {
            // Between cyan and yellow
            hue = 2.0f + static_cast<float>(b - r) / delta;
        } else {
            // Between magenta and cyan
            hue = 4.0f + static_cast<float>(r - g) / delta;
        }

        // Convert to degrees
        hue *= 60.0f;
        if (hue < 0.0f) hue += 360.0f;

        h = static_cast<uint16_t>(hue);
    }

    hsv_valid = true;
}
