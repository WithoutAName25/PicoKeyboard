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
        usb/main.cpp
        usb/tusb_config.h
        usb/usb_descriptors.cpp
        usb/usb_descriptors.h
        usb/main.h
        games/pong/PongGame.cpp
        games/pong/PongGame.h
        keys/KeyStateManager.cpp
        keys/KeyStateManager.h
        util/Scheduler.cpp
        util/Scheduler.h
        keys/KeyListener.cpp
        keys/KeyListener.h
)

pico_add_extra_outputs(PicoKeyboard)

target_link_libraries(PicoKeyboard pico_stdlib hardware_pio hardware_pwm LCDLibrary pico_unique_id tinyusb_device tinyusb_board)

target_include_directories(PicoKeyboard PUBLIC usb)

add_subdirectory(rgb)
