cmake_minimum_required(VERSION 3.27)

include(pico_sdk_import.cmake)

project(PicoKeyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR AND NOT DEFINED KEYBOARD_CONFIG_SOURCES)
    set(KEYBOARD_CONFIG_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/example/MyCustomKeyboardConfig.cpp
    )
endif ()

add_library(PicoKeyboard
        include/PicoKeyboard.h
        src/main.h
        ${KEYBOARD_CONFIG_SOURCES}
)

add_subdirectory(lib)
add_subdirectory(src)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

function(create_keyboard_side SIDE)
    add_executable(PicoKeyboard${SIDE}
            src/main.cpp)
    target_link_libraries(PicoKeyboard${SIDE} PRIVATE PicoKeyboard)
    string(TOUPPER "${SIDE}" SIDE_UPPERCASE)
    target_compile_definitions(PicoKeyboard${SIDE}
            PUBLIC KEYBOARD_${SIDE_UPPERCASE})
    pico_add_extra_outputs(PicoKeyboard${SIDE})
endfunction()

create_keyboard_side(Primary)
create_keyboard_side(Secondary)