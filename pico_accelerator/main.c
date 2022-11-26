#include <stdio.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

// TODO : GET SPEED, HUMP HEIGHT, DISTANCE, BARCODE DATA
volatile uint8_t count = 0;
bool repeating_timer_callback(struct repeating_timer *t)
{
    char text[16];
    sprintf(text, "3%d,4%d,6%d,1123%d\n", count, count, count, count);
    uart_puts(UART_ID, "AX");

    // For simulating changing data
    count++;
    if (count == 5)
    {
        count = 0;
    }
    return true;
}

// RX interrupt handler
void on_uart_rx()
{
    // print on m5 console
    uart_puts(UART_ID, "A");
    // negative flag
    uint8_t flagNegative = 0;

    // Declare variables
    int8_t x = -6;
    int8_t y = -6;

    while (uart_is_readable(UART_ID))
    {
        // value from m5 stick
        char ch = uart_getc(UART_ID);

        // Negative symbol (-) detected
        if ((uint8_t)ch == 45)
        {
            // Set flag
            flagNegative = 1;
        }

        else if (flagNegative == 1)
        {
            // Set negative number to x or y
            if (x == -6)
            {
                // convert ascii char to int
                x = ch - '0';

                // Negate value
                x *= -1;
            }
            else
            {
                // convert ascii char to int
                y = ch - '0';

                // Negate value
                y *= -1;
            }

            // reset flag
            flagNegative = 0;
        }

        else
        {
            // Set positivie number to x or y
            if (x == -6)
            {
                // convert ascii char to int
                x = ch - '0';
            }
            else
            {
                // convert ascii char to int
                y = ch - '0';
            }
        }

        // call mapping gotoNode if x and y are set
        // two dgit number
        if (x != -6 && y != -6)
        {
            // TODO: Use this
            // gotoNode(int x, int y)

            char text[6];
            sprintf(text, "%d,%d\n", x, y);
            // uart_puts(UART_ID, text);
            x = -6;
            y = -6;
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
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

    while (1)
    {
        tight_loop_contents();
    }

    // Kill repeating timer
    cancel_repeating_timer(&timer);
}
