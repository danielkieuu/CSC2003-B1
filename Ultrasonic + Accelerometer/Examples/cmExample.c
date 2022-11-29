//This example uses the ultrasonic script to get centimeters and writes it to UART

#include "pico/stdlib.h"
#include <stdio.h>
#include "ultrasonic.h"
#include "hardware/uart.h"


int main()
{
    stdio_init_all();
    setupUltrasonicPins();
    // testing purposes
    // while (true) { 
    //     printf("\n %d cm", scanLeft()); 
    //     sleep_ms(100);
    // }
    scanForward(); //function to scan forward 
    scanLeft(); //function to scan left
    scanRight(); //function to scan right
}