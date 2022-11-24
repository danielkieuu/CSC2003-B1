#include <stdio.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

volatile int count = 0;

bool repeating_timer_callback(struct repeating_timer *t)
{
    char text[100];
    sprintf(text, "1,5,2,6,%d\n", count);
    uart_puts(UART_ID, text);
    count++;
    return true;
}

// RX interrupt handler
void on_uart_rx()
{
    uint8_t flagNegative = 0;
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
                uart_puts(UART_ID, "\nHello, Negative Number\n");
                flagNegative = 0;
            }
            else
            { // Positive number 1st character E.g. 0 - 5
                uart_putc(UART_ID, ch);
                uart_puts(UART_ID, "\nHello, Positive Number\n");
            }
        }
    }
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

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // Send characters without conversion
    uart_puts(UART_ID, "Starting UART\n");

    // Set up repeating timer
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (1)
    {
        tight_loop_contents();
    }

    // Kill repeating timer
    cancel_repeating_timer(&timer);
}
