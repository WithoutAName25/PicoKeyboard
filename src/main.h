#pragma once

#include <keys.h>
#include <LCDLibrary.h>
#include <rgb.h>
#include <hardware/spi.h>
#include <pico/stdlib.h>

#include <utility>

struct DisplayConfig {
    spi_inst* spiInst;
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

    DisplayConfig(spi_inst* spiInst,
                  const uint8_t dataPin,
                  const uint8_t clockPin,
                  const uint chipSelectPin,
                  const uint dataCommandPin,
                  const uint resetPin,
                  const uint backlightPin,
                  const uint8_t hwXOffset,
                  const uint8_t hwYOffset,
                  const uint8_t hwWidth,
                  const uint8_t hwHeight,
                  const Rotation rotation)
        : spiInst(spiInst),
          dataPin(dataPin),
          clockPin(clockPin),
          chipSelectPin(chipSelectPin),
          dataCommandPin(dataCommandPin),
          resetPin(resetPin),
          backlightPin(backlightPin),
          hwXOffset(hwXOffset),
          hwYOffset(hwYOffset),
          hwWidth(hwWidth),
          hwHeight(hwHeight),
          rotation(rotation) {}
};

struct PicoKeyboardDeviceConfig {
    uint8_t numHwKeys;
    HWKeyConfig* hwKeys;
    uint8_t ledPin;
    uint8_t numLEDs;
    LedConfig* leds;
    DisplayConfig display;
    uart_inst* uart;
    uint8_t txPin;
    uint8_t rxPin;

    PicoKeyboardDeviceConfig(const uint8_t numHwKeys,
                             HWKeyConfig* hwKeys,
                             const uint8_t ledPin,
                             const uint8_t numLEDs,
                             LedConfig* leds,
                             const DisplayConfig& display,
                             uart_inst* uart,
                             const uint8_t txPin,
                             const uint8_t rxPin)
        : numHwKeys(numHwKeys),
          hwKeys(hwKeys),
          ledPin(ledPin),
          numLEDs(numLEDs),
          leds(leds),
          display(display),
          uart(uart),
          txPin(txPin),
          rxPin(rxPin) {}
};

struct PicoKeyboardConfig {
    uint8_t totalNumKeys;

    void (*configureKeys)(KeyActionController&);

    std::shared_ptr<IRGBEffect> defaultEffect;

    bool isMirrored;

    PicoKeyboardDeviceConfig primary;
    PicoKeyboardDeviceConfig secondary;

    PicoKeyboardConfig(uint8_t totalNumKeys,
                       void (*configureKeys)(KeyActionController&),
                       std::shared_ptr<IRGBEffect> defaultEffect,
                       bool isMirrored,
                       PicoKeyboardDeviceConfig primary,
                       PicoKeyboardDeviceConfig secondary)
        : totalNumKeys(totalNumKeys),
          configureKeys(configureKeys),
          defaultEffect(std::move(defaultEffect)),
          isMirrored(isMirrored),
          primary(primary),
          secondary(secondary) {}
};

PicoKeyboardConfig getKeyboardConfig();
