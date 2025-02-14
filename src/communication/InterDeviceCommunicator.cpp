#include "InterDeviceCommunicator.h"

void InterDeviceCommunicator::tick() {
    while (uart_is_readable(uart)) {
        uint8_t data;
        uart_read_blocking(uart, &data, 1);
        inputBuffer.push_back(data);
    }
    while (!outputBuffer.empty() && uart_is_writable(uart)) {
        uart_write_blocking(uart, &outputBuffer.front(), 1);
        outputBuffer.pop_front();
    }
}

InterDeviceCommunicator::InterDeviceCommunicator(uart_inst* uart, const uint8_t txPin, const uint8_t rxPin)
    : uart(uart) {
    gpio_set_function(txPin, GPIO_FUNC_UART);
    gpio_set_function(rxPin, GPIO_FUNC_UART);

    gpio_set_pulls(rxPin, true, false);

    uart_init(uart, 115200);
}

void InterDeviceCommunicator::execute(absolute_time_t timestamp) {
    tick();
}

void InterDeviceCommunicator::send(const uint8_t data) {
    outputBuffer.push_back(data);
    tick();
}

void InterDeviceCommunicator::send(uint8_t* data, size_t size) {
    outputBuffer.insert(outputBuffer.end(), data, data + size);
    tick();
}

void InterDeviceCommunicator::send16(uint16_t data) {
    send(reinterpret_cast<uint8_t*>(&data), sizeof(data));
}

void InterDeviceCommunicator::send32(uint32_t data) {
    send(reinterpret_cast<uint8_t*>(&data), sizeof(data));
}

void InterDeviceCommunicator::send64(uint64_t data) {
    send(reinterpret_cast<uint8_t*>(&data), sizeof(data));
}

uint8_t InterDeviceCommunicator::peek() {
    while (inputBuffer.empty()) {
        sleep_us(10);
        tick();
    }
    const unsigned char& data = inputBuffer.front();
    return data;
}

uint8_t InterDeviceCommunicator::receive() {
    const uint8_t data = peek();
    inputBuffer.pop_front();
    return data;
}

void InterDeviceCommunicator::receive(uint8_t* buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = receive();
    }
}

uint16_t InterDeviceCommunicator::receive16() {
    uint16_t data;
    receive(reinterpret_cast<uint8_t*>(&data), sizeof(data));
    return data;
}

uint32_t InterDeviceCommunicator::receive32() {
    uint32_t data;
    receive(reinterpret_cast<uint8_t*>(&data), sizeof(data));
    return data;
}

uint64_t InterDeviceCommunicator::receive64() {
    uint64_t data;
    receive(reinterpret_cast<uint8_t*>(&data), sizeof(data));
    return data;
}

bool InterDeviceCommunicator::hasData() {
    tick();
    return !inputBuffer.empty();
}
