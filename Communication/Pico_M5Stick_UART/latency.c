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

int main()
{
    // start timer
    stdio_init_all();

    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    uint8_t send = 0;
    while (true)
    {
        tight_loop_contents();
        if (send == 0)
        {
            uart_putc(UART_ID, 'a');
            send++;
        }
    }
}
