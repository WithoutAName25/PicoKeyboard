cmake_minimum_required(VERSION 3.27)

include(pico_sdk_import.cmake)

project(PicoKeyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(lib)

add_executable(PicoKeyboard main.cpp)

target_link_libraries(PicoKeyboard pico_stdlib hardware_spi hardware_pwm LCDLibrary)
