#include "Motor Control/PID-PWM.h"
#include "Ultrasonic + Accelerometer/ultrasonic/ultrasonic.h"
#include "Mapping/mapping.c"
#include "Communication/Integration/main.c"

#define PI 3.14159

const int UART_TX = 0;
const int UART_RX = 1;
const int WHEEL_ENCODER_RIGHT = 5;
const int WHEEL_ENCODER_LEFT = 6;
const int ULTRA_FRONT_TRIG = 9;
const int ULTRA_FRONT_ECHO = 10;
const int ULTRA_LEFT_TRIG = 11;
const int ULTRA_LEFT_ECHO = 12;
const int ULTRA_RIGHT_TRIG = 14;
const int ULTRA_RIGHT_ECHO = 15;
const int PWM_LEFT = 16;
const int PWM_RIGHT = 17;
const int INPUT_A_LEFT = 18;
const int INPUT_B_LEFT = 19;
const int INPUT_A_RIGHT = 20;
const int INPUT_B_RIGHT = 21;
const int BARCODE_IR = 28;

const int DEGREE_PER_NOTCH = 9;
struct COMMS_DATA
{
    int speed;
    int hump;
    int dist;
    char barcode[3];
} COMMS_DATA;

int main()
{
    comms_main();
    COMMS_DATA.speed = 0;
    COMMS_DATA.hump = 0;
    COMMS_DATA.dist = 0;
    COMMS_DATA.barcode[0] = '0';
    Map();
}
