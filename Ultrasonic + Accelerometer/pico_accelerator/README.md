## Members

| Ang Quan Xing | 2101536 |
| Toh Zheng Xiang | 2101089 |
| Daniel Kieu | 2101231 |

## Sub component for accelerometer

#### Flowchart

Click here to view: https://drive.google.com/file/d/18v2qmqcGd1pVVukih6Ki_U0QFPf2-E_9/view?usp=sharing

## Pico Set-up

There are a few option listed to setup Pico:

1. Reference this Video Tutorial [Click Here](https://www.youtube.com/watch?v=B5rQSoOmR5w)
2. Reference this Written Guide [Click Here](https://www.digikey.sg/en/maker/projects/raspberry-pi-pico-and-rp2040-cc-part-1-blink-and-vs-code/7102fb8bca95452e9df6150f39ae8422)
3. Reference this Official Guide, Chapter 9.2 for guide on setting up on Windows [Click Here](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)

## M5StickC PLUS

M5StickC Plus Quick Start: https://docs.m5stack.com/en/quick_start/m5stickc_plus/uiflow

Only `Ground`, `G32`, and `G33` pins are required to connect to `Ground`, `P3.2`, and `P3.3` respectively.

on https://flow.m5stack.com/. switch to python mode and copy paste the context from `accerelometer.txt` to run the m5stick with preconfigure python program.

## Projects

pico_accelerometer

1. Send accelerometer data from IMU sensor located on M5stick CPlus to Raspberry PICO through i2c
2. Kalman filter algo is used to improve and reduce the noise from the IMU sensor
