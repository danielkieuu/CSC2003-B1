#include <stdio.h>
#include <time.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"

// UART CONFIG
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE
#define UART_TX_PIN 0
#define UART_RX_PIN 1

/**
 * Clock is not implemented in pico sdk
 * Is implemented using time_us_64()
 * @brief Get the current time in seconds
 */
clock_t clock()
{
    return (clock_t)time_us_64() / 10000;
}

int main()
{
    stdio_init_all();
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    int count = 0;
    while (true)
    {
        tight_loop_contents();

        // start timer
        clock_t startTime = clock();

        // send 65KB
        while (count < 65000)
        {
            uart_putc(UART_ID, 'a');
            count++;
            printf("Count: %d", count);
        }

        if (count == 65000)
        {
            // stop timer
            clock_t endTime = clock();

            // calculate time taken in seconds
            double executionTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

            // print time taken
            printf("%.8f sec\n", executionTime);

            // stop counter
            count = 65001;
        }
    }
}
