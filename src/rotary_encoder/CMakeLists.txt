file(GLOB_RECURSE ROTARY_ENCODER_SOURCES "*.cpp")
file(GLOB_RECURSE ROTARY_ENCODER_HEADERS "*.h")

add_library(rotary_encoder_lib ${ROTARY_ENCODER_SOURCES} ${ROTARY_ENCODER_HEADERS})

target_link_libraries(rotary_encoder_lib pico_stdlib)

target_include_directories(rotary_encoder_lib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)