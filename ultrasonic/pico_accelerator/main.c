#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

static const double Q_angle = 0.001, Q_gyro = 0.003, R_angle = 0.5, dtt = 0.005, C_0 = 1;
double P[2][2] = {{1, 0}, {0, 1}};
double Pdot[4] = {0, 0, 0, 0};
double q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
bool flag = false;

// TODO : GET SPEED, HUMP HEIGHT, DISTANCE, BARCODE DATA
volatile uint8_t count = 0;
float angleX, angleY, angleZ;

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

// Kalman filter algo
double Kalman_Filter(double angle_m, double gyro_m)
{
    // Step 1
    angleX += (gyro_m - q_bias) * dtt;

    // Step 2:
    Pdot[0] = Q_angle - P[0][1] - P[1][0];
    Pdot[1] = -P[1][1];
    Pdot[2] = -P[1][1];
    Pdot[3] = Q_gyro;
    P[0][0] += Pdot[0] * dtt;
    P[0][1] += Pdot[1] * dtt;
    P[1][0] += Pdot[2] * dtt;
    P[1][1] += Pdot[3] * dtt;
    angle_err = angle_m - angleX;
    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];
    E = R_angle + C_0 * PCt_0;
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];
    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
    angleX += K_0 * angle_err;
    q_bias += K_1 * angle_err;
    return angleX;
}

bool detectAngleOfElevation(double angle)
{
    if (angle > 28)
    {
        flag = true;
    }
    return flag;
}

// RX interrupt handler
void on_uart_rx()
{
    // print on m5 console
    char buffer[50];
    int a = 10, b = 20, c;
    c = a + b;
    sprintf(buffer, "Sum of %d and %d is %d", a, b, c);
    uart_puts(UART_ID, buffer);
    // negative flag
    uint8_t flagNegative = 0;

    // Declare variables
    int8_t x = -6;
    int8_t y = -6;
    double kalmanAngle;

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

        // Form 4 byte to float
        if (x != -6 && y != -6)
        {
            // TODO: Use this
            // gotoNode(int x, int y)

            char text[6];
            sprintf(text, "%d,%d %d %d\n", x, y);
            kalmanAngle = atof(text);

            if (detectAngleOfElevation(kalmanAngle))
            {
                // call PID function to speed up
            }

            // kalmanAngle = Kalman_Filter(kalmanAngle);

            // print back in serail console
            uart_puts(UART_ID, text);
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
    char buffer[50];
    int a = 10, b = 20, c;
    c = a + b;
    sprintf(buffer, "Sum of %d and %d is %d", a, b, c);
    uart_puts(UART_ID, buffer);

    // The string "sum of 10 and 20 is 30" is stored
    // into buffer instead of printing on stdout
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
