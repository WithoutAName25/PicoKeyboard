#pragma once

#include "pico/stdlib.h"
#include "util.h"
#include <list>

class InterDeviceCommunicator final : public IExecutable {
    uart_inst_t *uart;
    std::list<uint8_t> inputBuffer;
    std::list<uint8_t> outputBuffer;
    uint8_t bytesInFlight = 0;

    void tick();

public:
    InterDeviceCommunicator(uart_inst *uart, uint8_t txPin, uint8_t rxPin);

    void execute(absolute_time_t timestamp) override;

    void send(uint8_t data);

    void send(uint8_t *data, size_t size);

    void send16(uint16_t data);

    void send32(uint32_t data);

    void send64(uint64_t data);

    [[nodiscard]] uint8_t peek();

    uint8_t receive();

    void receive(uint8_t *buf, size_t size);

    uint16_t receive16();

    uint32_t receive32();

    uint64_t receive64();

    bool hasData();
};
