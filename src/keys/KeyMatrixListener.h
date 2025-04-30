#pragma once
#include <map>
#include <set>

#include "IKeyListener.h"

struct HWMatrixKeyConfig {
    uint8_t id;
    uint8_t pinIn;
    uint8_t pinOut;

    HWMatrixKeyConfig(const uint8_t id, const uint8_t pinIn, const uint8_t pinOut)
        : id(id), pinIn(pinIn), pinOut(pinOut) {}
};

class KeyMatrixListener final : public IKeyListener {
    std::map<uint8_t, std::vector<HWMatrixKeyConfig*>> keysPerInPin;
    std::set<uint8_t> outPins;

public:
    KeyMatrixListener(KeyStateController& controller, uint8_t numKeys, HWMatrixKeyConfig* keys);

    void setupPins() override;

    void execute(absolute_time_t timestamp) override;
};
