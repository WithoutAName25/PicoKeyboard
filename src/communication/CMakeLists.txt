file(GLOB_RECURSE COMMUNICATION_SOURCES "*.cpp")
file(GLOB_RECURSE COMMUNICATION_HEADERS "*.h")

add_library(communication_lib ${COMMUNICATION_SOURCES} ${COMMUNICATION_HEADERS})

target_link_libraries(communication_lib pico_stdlib hardware_uart keys_lib util_lib)

target_include_directories(communication_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)