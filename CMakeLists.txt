cmake_minimum_required(VERSION 3.27)

include(pico_sdk_import.cmake)

project(PicoKeyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(lib)

add_executable(PicoKeyboard main.cpp
        rgb/rgb.cpp
        rgb/rgb.h
        rgb/RGBController.cpp
        rgb/RGBController.h
        rotary_encoder/RotaryEncoder.cpp
        rotary_encoder/RotaryEncoder.h
        keys/KeyManager.cpp
        keys/KeyManager.h)

pico_enable_stdio_usb(PicoKeyboard 1)

pico_add_extra_outputs(PicoKeyboard)

target_link_libraries(PicoKeyboard pico_stdlib hardware_pio hardware_pwm LCDLibrary)

add_subdirectory(rgb)
