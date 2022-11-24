#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Measure Execution Time of Code
clock_t clock()
{
    return (clock_t)time_us_64() / 10000;
}

int main()
{
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    while (true)
    {
        int counterCycle =0;
        clock_t startTime = clock(); // Measure Execution Time of Code
        clock_t endTime = clock();

        double executionTime = (endTime - startTime) / CLOCKS_PER_SEC;

        while (executionTime < 1.0)
        {
            uart_putc(UART_ID, 'A');
            endTime = clock();
            counterCycle++;
            executionTime = (endTime - startTime) / CLOCKS_PER_SEC;
        }
        char text[12];
        sprintf(text, "Counter: %d", counterCycle);
        uart_puts(UART_ID, text);
        printf("Counter: %d", counterCycle);
    }

}