file(GLOB_RECURSE UTIL_SOURCES "*.cpp")
file(GLOB_RECURSE UTIL_HEADERS "*.h")

add_library(util_lib ${UTIL_SOURCES} ${UTIL_HEADERS})

target_link_libraries(util_lib pico_stdlib)

target_include_directories(util_lib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)