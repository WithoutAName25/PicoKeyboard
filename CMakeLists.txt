cmake_minimum_required(VERSION 3.27)

include(pico_sdk_import.cmake)

project(PicoKeyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(PicoKeyboard
        src/main.cpp
)

add_subdirectory(lib)
add_subdirectory(src)

pico_add_extra_outputs(PicoKeyboard)

target_link_libraries(PicoKeyboard
        pico_stdlib
        LCDLibrary
        pong_lib
        keys_lib
        rgb_lib
        rotary_encoder_lib
        usb_lib
        util_lib)

target_include_directories(PicoKeyboard
        PRIVATE
        ${CMAKE_SOURCE_DIR})