add_executable(ultrasonic cmExample.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(ultrasonic pico_stdlib hardware_uart pico_ultrasonic)

pico_enable_stdio_usb(ultrasonic 1)
pico_enable_stdio_uart(ultrasonic 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(ultrasonic)

# add url via pico_set_program_url
example_auto_set_url(ultrasonic)


