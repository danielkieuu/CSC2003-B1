#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "PID-PWM.h"

const int INPUT_A_LEFT = 18;
const int INPUT_B_LEFT = 19;
const int INPUT_A_RIGHT = 20;
const int INPUT_B_RIGHT = 21;
const int WHEEL_ENCODER_RIGHT = 13;
const int WHEEL_ENCODER_LEFT = 8;
const int PWM_LEFT = 15;
const int PWM_RIGHT = 14;

float wheel_left_rpm = 0.0;
float wheel_right_rpm = 0.0;

int PWM_RIGHT_CYCLE = 32768;
int PWM_LEFT_CYCLE = 32768;


PID pid_control, *pid_ctrl_ptr;

void PID_Coefficents(struct PID *pid_struct, float Kp, float Ki, float Kd, float tau, float intergrator_lmt) {
    pid_struct->Kp = Kp;
    pid_struct->Ki = Ki;
    pid_struct->Kd = Kd;
    pid_struct->tau = tau;
    pid_struct->integrator_lmt = intergrator_lmt;
    pid_struct->time_prev = get_absolute_time();
}

void change_pwm(float difference){
    int pwm_duty_cycle_per_rpm = 182; //Calibrated value
    PWM_LEFT_CYCLE = round(difference * pwm_duty_cycle_per_rpm) + PWM_LEFT_CYCLE;
}

void stop_movement(){
    //Stops all movement (directional or otherwise)
    gpio_put(INPUT_A_LEFT, 0);
    gpio_put(INPUT_A_RIGHT, 0);
    gpio_put(INPUT_B_LEFT, 0);
    gpio_put(INPUT_B_RIGHT, 0);
}

void turn_left(){
    gpio_put(INPUT_A_LEFT, 1);
    gpio_put(INPUT_A_RIGHT, 0);
    gpio_put(INPUT_B_LEFT, 0);
    gpio_put(INPUT_B_RIGHT, 1);
}

void turn_right(){
    gpio_put(INPUT_A_LEFT, 0);
    gpio_put(INPUT_A_RIGHT, 1);
    gpio_put(INPUT_B_LEFT, 1);
    gpio_put(INPUT_B_RIGHT, 0);
}

void move_forward() {
    // Set channel A & B output high for PWM_RIGHT_CYCLE & PWM_LEFT_CYCLE number of cycles
    if (!gpio_get(INPUT_B_LEFT) && !gpio_get(INPUT_B_RIGHT)){
        gpio_put(INPUT_B_LEFT, 1);
        gpio_put(INPUT_B_RIGHT, 1);
    } 
    pwm_clear_irq(pwm_gpio_to_slice_num(PWM_RIGHT));
    pwm_set_gpio_level(PWM_RIGHT, PWM_RIGHT_CYCLE);
    pwm_set_gpio_level(PWM_LEFT, PWM_LEFT_CYCLE);
}



void wheel_speed_right(){
    static int counter_right = 0;
    counter_right += 1;
    if (counter_right == 20){
        wheel_right_rpm += 1.0;
        counter_right = 0;
        pid_ctrl_ptr = &pid_control;
        pid_ctrl_ptr->setpoint = wheel_right_rpm;
        pid_ctrl_ptr->measured = wheel_left_rpm;
        float diff = PID_Compute(get_absolute_time(), pid_ctrl_ptr);
        wheel_left_rpm += diff;
        change_pwm(diff);
    }
}

void wheel_speed_left(){
    static int counter_left = 0;
    counter_left += 1;
    if (counter_left == 20){
        wheel_left_rpm += 1.0;
        counter_left = 0;
    }
    
    
}

void gpio_callback(uint gpio, uint32_t events){
    if (gpio==WHEEL_ENCODER_RIGHT){
        wheel_speed_right();
    }
    if (gpio==WHEEL_ENCODER_LEFT){
        wheel_speed_left();
    }
}

float PID_Compute(absolute_time_t time, struct PID *pid) {
    float time_delta = absolute_time_diff_us(pid->time_prev, time) / 1000000.f; // seconds
    // printf("Time delta: %f\n", time_delta);
    float error = pid->setpoint - pid->measured;

    // ~~~ Proportianal ~~~
    float proportianal = pid->Kp * error;


    // ~~~ Intergral ~~~
    // (previous + current*0.5)
    float intergral  = pid->integrator + (0.5f * pid->Ki * pid->prev_err * time_delta);

    // Clamp it to prevent wind-up https://en.wikipedia.org/wiki/Integral_windup
    if(intergral > +(pid->integrator_lmt) ) 
        intergral = +pid->integrator_lmt;
    else if (intergral < -(pid->integrator_lmt) )
        intergral = -pid->integrator_lmt;

    pid->integrator = intergral;


    // ~~~ Differential ~~~ 
    // Band limited differentiator using the tau term
    // Differential of the measured term, prevents BIG jumps on setpoint change
    float err_diff = -(2.0f * pid->Kd * (pid->measured - pid->prev_measured)
                     +(2.0f * pid->tau - time_delta) * pid->differentiator)
                     /(2.0f * pid->tau + time_delta);

    // Store what we need for next time
    pid->prev_err = error; 
    pid->time_prev = time;
    pid->prev_measured = pid->measured;

    // Output is PID equation: Kp*error + Ki*error_integrated + Kd*error_differential
    float output = proportianal + pid->integrator + pid->differentiator;
    return (output);
}

int main()
{
    stdio_init_all();
    //Init PID struct
    PID_Coefficents(&pid_control, 0.5, 0.05, 0.01, 0.02, 1);

    //Init GPIO required
    //GPIO 7 & 8 for wheel encoders
    //GPIO 18-21 for wheel direction (18+19 for wheel a, 20+21 for wheel b)
    gpio_init(WHEEL_ENCODER_RIGHT);
    gpio_init(WHEEL_ENCODER_LEFT);
    gpio_init(INPUT_A_LEFT);
    gpio_init(INPUT_B_LEFT);
    gpio_init(INPUT_A_RIGHT);
    gpio_init(INPUT_B_RIGHT);

    // Tell GPIO 14 and 15 they are allocated to the PWM
    gpio_set_function(PWM_RIGHT, GPIO_FUNC_PWM);
    gpio_set_function(PWM_LEFT, GPIO_FUNC_PWM);

    //Set GPIO to in for 7 & 8
    //Set GPIO to out for GPIO 18-21 (controls wheel direction)
    gpio_set_dir(WHEEL_ENCODER_RIGHT, GPIO_IN);
    gpio_set_dir(WHEEL_ENCODER_LEFT, GPIO_IN);
    gpio_set_dir(INPUT_A_LEFT, GPIO_OUT);
    gpio_set_dir(INPUT_B_LEFT, GPIO_OUT);
    gpio_set_dir(INPUT_A_RIGHT, GPIO_OUT);
    gpio_set_dir(INPUT_B_RIGHT, GPIO_OUT);

    //Set GPIO 18-21 to LOW
    gpio_put(INPUT_A_LEFT, 0);
    gpio_put(INPUT_B_LEFT, 0);
    gpio_put(INPUT_A_RIGHT, 0);
    gpio_put(INPUT_B_RIGHT, 0);

    //Pull on 7 & 8
    gpio_pull_up(WHEEL_ENCODER_RIGHT);
    gpio_pull_up(WHEEL_ENCODER_LEFT);
    
    // Find out which PWM slice is connected to GPIO 14
    uint slice_num = pwm_gpio_to_slice_num(PWM_RIGHT);

    //Enabled Interrupts on GPIO 7 & 8
    gpio_set_irq_enabled_with_callback(WHEEL_ENCODER_RIGHT, GPIO_IRQ_EDGE_FALL, 1, &gpio_callback);
    gpio_set_irq_enabled(WHEEL_ENCODER_LEFT, GPIO_IRQ_EDGE_FALL, 1);

    //Setup PWM and interrupt
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, move_forward);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Get PWM config
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice_num, &config, true);

    while(1){
        tight_loop_contents();
    };

    return 0;
}
