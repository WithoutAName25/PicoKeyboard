file(GLOB_RECURSE USB_SOURCES "*.cpp")
file(GLOB_RECURSE USB_HEADERS "*.h")

add_library(usb_lib ${USB_SOURCES} ${USB_HEADERS})

target_link_libraries(usb_lib pico_stdlib tinyusb_device tinyusb_board keys_lib util_lib)

target_include_directories(usb_lib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)