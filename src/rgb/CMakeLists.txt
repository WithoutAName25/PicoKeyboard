file(GLOB_RECURSE RGB_SOURCES "*.cpp")
file(GLOB_RECURSE RGB_HEADERS "*.h")

add_library(rgb_lib ${RGB_SOURCES} ${RGB_HEADERS})

pico_generate_pio_header(rgb_lib ${CMAKE_CURRENT_SOURCE_DIR}/neopixel.pio)

target_link_libraries(rgb_lib pico_stdlib hardware_pio keys_lib)

target_include_directories(rgb_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)