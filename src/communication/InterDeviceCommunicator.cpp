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

InterDeviceCommunicator::InterDeviceCommunicator(uart_inst *uart, const uint8_t txPin, const uint8_t rxPin)
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

uint8_t InterDeviceCommunicator::peek() const {
    while (inputBuffer.empty()) {
        sleep_us(100);
    }
    const unsigned char &data = inputBuffer.front();
    return data;
}

uint8_t InterDeviceCommunicator::receive() {
    const uint8_t data = peek();
    inputBuffer.pop_front();
    return data;
}

bool InterDeviceCommunicator::hasData() {
    tick();
    return !inputBuffer.empty();
}
