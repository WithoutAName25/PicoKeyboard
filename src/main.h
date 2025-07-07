#pragma once

#include <LCDLibrary.h>
#include <hardware/spi.h>
#include <keys.h>
#include <pico/stdlib.h>
#include <rgb.h>

#include <utility>

extern RGBController rgbController;
extern KeyStateController keyStateController;

struct DisplayConfig {
    spi_inst *spiInst;
    uint8_t dataPin;
    uint8_t clockPin;
    uint chipSelectPin;
    uint dataCommandPin;
    uint resetPin;
    uint backlightPin;
    uint8_t hwXOffset;
    uint8_t hwYOffset;
    uint8_t hwWidth;
    uint8_t hwHeight;
    Rotation rotation;

    DisplayConfig(spi_inst *spiInst, const uint8_t dataPin, const uint8_t clockPin, const uint chipSelectPin,
                  const uint dataCommandPin, const uint resetPin, const uint backlightPin, const uint8_t hwXOffset,
                  const uint8_t hwYOffset, const uint8_t hwWidth, const uint8_t hwHeight, const Rotation rotation)
        : spiInst(spiInst), dataPin(dataPin), clockPin(clockPin), chipSelectPin(chipSelectPin),
          dataCommandPin(dataCommandPin), resetPin(resetPin), backlightPin(backlightPin), hwXOffset(hwXOffset),
          hwYOffset(hwYOffset), hwWidth(hwWidth), hwHeight(hwHeight), rotation(rotation) {}
};

struct PicoKeyboardDeviceConfig {
    uint8_t numHwKeys;
    HWKeyConfig *hwKeys;
    HWMatrixKeyConfig *hwMatrixKeys;
    uint8_t ledPin;
    uint8_t numLEDs;
    LedConfig *leds;
    DisplayConfig *display;
    uart_inst *uart;
    uint8_t txPin;
    uint8_t rxPin;

    PicoKeyboardDeviceConfig(const uint8_t numHwKeys, HWKeyConfig *hwKeys, const uint8_t ledPin, const uint8_t numLEDs,
                             LedConfig *leds, DisplayConfig *display = nullptr, uart_inst *uart = nullptr,
                             const uint8_t txPin = 0, const uint8_t rxPin = 0)
        : numHwKeys(numHwKeys), hwKeys(hwKeys), hwMatrixKeys(nullptr), ledPin(ledPin), numLEDs(numLEDs), leds(leds),
          display(display), uart(uart), txPin(txPin), rxPin(rxPin) {}

    PicoKeyboardDeviceConfig(const uint8_t numHwKeys, HWMatrixKeyConfig *hwMatrixKeys, const uint8_t ledPin,
                             const uint8_t numLEDs, LedConfig *leds, DisplayConfig *display = nullptr,
                             uart_inst *uart = nullptr, const uint8_t txPin = 0, const uint8_t rxPin = 0)
        : numHwKeys(numHwKeys), hwKeys(nullptr), hwMatrixKeys(hwMatrixKeys), ledPin(ledPin), numLEDs(numLEDs),
          leds(leds), display(display), uart(uart), txPin(txPin), rxPin(rxPin) {}
};

struct PicoKeyboardConfig {
    uint8_t totalNumKeys;

    void (*configureKeys)(KeyActionController &);
    std::function<void(
        const std::function<void(absolute_time_t time, std::function<void(absolute_time_t timestamp)> block)> &exec)>
        configureStartup;

    bool isMirrored;

    PicoKeyboardDeviceConfig *primary;
    PicoKeyboardDeviceConfig *secondary;

    PicoKeyboardConfig(
        const uint8_t totalNumKeys, void (*configureKeys)(KeyActionController &),
        std::function<void(const std::function<void(absolute_time_t time,
                                                    std::function<void(absolute_time_t timestamp)> block)> &exec)>
            configureStartup,
        PicoKeyboardDeviceConfig *primary)
        : totalNumKeys(totalNumKeys), configureKeys(configureKeys), configureStartup(std::move(configureStartup)),
          isMirrored(false), primary(primary), secondary(nullptr) {}

    PicoKeyboardConfig(
        const uint8_t totalNumKeys, void (*configureKeys)(KeyActionController &),
        std::function<void(const std::function<void(absolute_time_t time,
                                                    std::function<void(absolute_time_t timestamp)> block)> &exec)>
            configureStartup,
        const bool isMirrored, PicoKeyboardDeviceConfig *primary, PicoKeyboardDeviceConfig *secondary)
        : totalNumKeys(totalNumKeys), configureKeys(configureKeys), configureStartup(std::move(configureStartup)),
          isMirrored(isMirrored), primary(primary), secondary(secondary) {}
};

PicoKeyboardConfig getKeyboardConfig();
