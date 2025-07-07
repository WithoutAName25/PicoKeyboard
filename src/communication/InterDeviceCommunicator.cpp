#include "InterDeviceCommunicator.h"

#include "command/Command.h"

void InterDeviceCommunicator::tick() {
    uint8_t received = 0;
    while (uart_is_readable(uart)) {
        uint8_t controlPacket;
        uart_read_blocking(uart, &controlPacket, 1);
        const uint8_t num = controlPacket & 0x7F;
        if (controlPacket & 0x80) {
            uint8_t data;
            for (int i = 0; i < num; ++i) {
                uart_read_blocking(uart, &data, 1);
                inputBuffer.push_back(data);
            }
            received += num;
        } else {
            if (bytesInFlight < num) {
                bytesInFlight = 0;
            } else {
                bytesInFlight -= num;
            }
        }
    }
    if (received > 0) {
        uart_write_blocking(uart, &received, 1);
    }
    const uint8_t toSend = std::min<uint32_t>(24 - bytesInFlight, outputBuffer.size());
    if (toSend == 0)
        return;
    bytesInFlight += toSend;
    const uint8_t data = toSend | 0x80;
    uart_write_blocking(uart, &data, 1);
    for (uint8_t i = 0; i < toSend; ++i) {
        uart_write_blocking(uart, &outputBuffer.front(), 1);
        outputBuffer.pop_front();
    }
}

InterDeviceCommunicator::InterDeviceCommunicator(uart_inst *uart, const uint8_t txPin, const uint8_t rxPin)
    : uart(uart) {
    if (uart == nullptr)
        return;
    gpio_set_function(txPin, GPIO_FUNC_UART);
    gpio_set_function(rxPin, GPIO_FUNC_UART);

    gpio_set_pulls(rxPin, true, false);

    uart_init(uart, 115200);
}

void InterDeviceCommunicator::execute(absolute_time_t timestamp) {
    if (uart == nullptr)
        return;
    tick();
}

void InterDeviceCommunicator::send(const uint8_t data) {
    if (uart == nullptr)
        return;
    outputBuffer.push_back(data);
    tick();
}

void InterDeviceCommunicator::send(uint8_t *data, const size_t size) {
    if (uart == nullptr)
        return;
    outputBuffer.insert(outputBuffer.end(), data, data + size);
    tick();
}

void InterDeviceCommunicator::send16(uint16_t data) { send(reinterpret_cast<uint8_t *>(&data), sizeof(data)); }

void InterDeviceCommunicator::send32(uint32_t data) { send(reinterpret_cast<uint8_t *>(&data), sizeof(data)); }

void InterDeviceCommunicator::send64(uint64_t data) { send(reinterpret_cast<uint8_t *>(&data), sizeof(data)); }

uint8_t InterDeviceCommunicator::peek() {
    if (uart == nullptr)
        return 0;
    while (inputBuffer.empty()) {
        sleep_us(10);
        tick();
    }
    const unsigned char &data = inputBuffer.front();
    return data;
}

uint8_t InterDeviceCommunicator::receive() {
    if (uart == nullptr)
        return 0;

    const uint8_t data = peek();
    inputBuffer.pop_front();
    return data;
}

void InterDeviceCommunicator::receive(uint8_t *buf, const size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = receive();
    }
}

uint16_t InterDeviceCommunicator::receive16() {
    uint16_t data;
    receive(reinterpret_cast<uint8_t *>(&data), sizeof(data));
    return data;
}

uint32_t InterDeviceCommunicator::receive32() {
    uint32_t data;
    receive(reinterpret_cast<uint8_t *>(&data), sizeof(data));
    return data;
}

uint64_t InterDeviceCommunicator::receive64() {
    uint64_t data;
    receive(reinterpret_cast<uint8_t *>(&data), sizeof(data));
    return data;
}

bool InterDeviceCommunicator::hasData() {
    if (uart == nullptr)
        return false;

    tick();
    return !inputBuffer.empty();
}
