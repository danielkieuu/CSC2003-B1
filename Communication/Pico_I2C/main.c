#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <stdlib.h>

static int addr = 0x42;
// Ports
i2c_inst_t *i2c = i2c0;

int main()
{
    // Initialize chosen serial port
    stdio_init_all();

    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);

    // PICO_DEFAULT_I2C_SDA_PIN definition found here: https://github.com/raspberrypi/pico-sdk/blob/master/src/boards/include/boards/pico.h
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); // GPIO Pin 4
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); // GPIO Pin 5
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);                     // GPIO Pin 4
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);                     // GPIO Pin 5

    // i2c_default definition found here: https://raspberrypi.github.io/pico-sdk-doxygen/rp2__common_2hardware__i2c_2include_2hardware_2i2c_8h_source.html
    i2c_init(i2c0, 100 * 1000); // use i2c0 controller, 100kHz clock
    i2c_set_slave_mode(i2c0, true, 0x42);

    uint8_t reg_data[1];
    reg_data[0] = 97;

    while (true)
    {
        i2c_write_raw_blocking(i2c0, reg_data, 1);
    }

    return 0;
}