cmake_minimum_required(VERSION 3.27)

include(pico_sdk_import.cmake)

project(PicoKeyboardConfig C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Configure the config sources
set(KEYBOARD_CONFIG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCustomKeyboardConfig.cpp
)

# Add PicoKeyboard directory
add_subdirectory(PicoKeyboard)