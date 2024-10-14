file(GLOB_RECURSE KEYS_SOURCES "*.cpp")
file(GLOB_RECURSE KEYS_HEADERS "*.h")

add_library(keys_lib ${KEYS_SOURCES} ${KEYS_HEADERS})

target_link_libraries(keys_lib
        PUBLIC
        pico_stdlib
        communication_lib
        rgb_lib
        usb_lib
        util_lib)

target_include_directories(keys_lib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)