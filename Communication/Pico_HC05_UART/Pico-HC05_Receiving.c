
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Negative number flag
int flagNegative = 0;

// RX interrupt handler
void on_uart_rx()
{
    clock_t startTime = clock(); // Measure Execution Time of Code
    clock_t endTime = clock();
    while (uart_is_readable(UART_ID))
    {
        uint8_t ch = uart_getc(UART_ID);

        if (uart_is_writable(UART_ID))
        {
            if (ch == 45)
            { // Negative number 1st character E.g. -5 to -1, (-)
                flagNegative = 1;
                uart_putc(UART_ID, ch);
            }
            else if (flagNegative == 1)
            { // Negative number 2nd character E.g. -5 to -1, (5)
                uart_putc(UART_ID, ch);
                uart_puts(UART_ID, " - Negative Number\n\r");
                flagNegative = 0;
            }
            else
            { // Positive number 1st character E.g. 0 - 5
                uart_putc(UART_ID, ch);
                uart_puts(UART_ID, " - Positive Number\n\r");
            }
        }
        endTime = clock();
        if (ch != 45)
        {
            double executionTime = (double)(endTime - startTime) / (double)CLOCKS_PER_SEC;
            printf("\n\rExecution Time of Code: %.4f seconds\n\r", executionTime);
        }
    }
}

// Measure Execution Time of Code
clock_t clock()
{
    return (clock_t)time_us_64() / 10000;
}

int main()
{
    stdio_init_all();

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    while (1)
        tight_loop_contents();
}