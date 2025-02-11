#pragma once

#include <list>
#include "pico/stdlib.h"
#include "util.h"

class InterDeviceCommunicator final : public IExecutable {
    uart_inst_t *uart;
    std::list<uint8_t> inputBuffer;
    std::list<uint8_t> outputBuffer;

    void tick();

public:
    InterDeviceCommunicator(uart_inst *uart, uint8_t txPin, uint8_t rxPin);

    void execute(absolute_time_t timestamp) override;

    void send(uint8_t data);

    [[nodiscard]] uint8_t peek() const;

    uint8_t receive();

    bool hasData();
};
