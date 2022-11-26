cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_accelerator C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

include(example_auto_set_url.cmake)

add_executable(${PROJECT_NAME}
    main.c
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# pull in common dependencies and additional i2c hardware support
target_link_libraries(${PROJECT_NAME} pico_stdlib)

# FOR DEBUGGING - Enable USB output, disable uart output
# Open serial terminal to view output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

add_compile_options(-Wall
    -Wno-format # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
    -Wno-maybe-uninitialized
)
