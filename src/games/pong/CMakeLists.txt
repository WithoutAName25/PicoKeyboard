file(GLOB_RECURSE PONG_SOURCES "*.cpp")
file(GLOB_RECURSE PONG_HEADERS "*.h")

add_library(pong_lib ${PONG_SOURCES} ${PONG_HEADERS})

target_link_libraries(pong_lib pico_stdlib LCDLibrary rotary_encoder_lib)

target_include_directories(pong_lib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)