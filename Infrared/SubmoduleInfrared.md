# Drunken Robotic Car - CSC2003 Team B1 Submodule Infrared (Encoder + Barcode)


## Raspberry Pi PICO
### In 2012, Raspberry Pi Foundation introduced us to a single board computer i.e. Raspberry Pi, and 9 years later, a microcontroller board, Raspberry Pi Pico was introduced. 
### Raspberry Pi Pico comes with the Raspberry Pi Foundation's own silicon chip RP2040. Because it has a Dual-Core ARM processor along with GPIO & Digital/Analog peripherals, we can easily create micro to small scale embedded applications. The Pico has a single 12-bit resolution, 4-channel ADC with a 3.3V internal reference and available inputs on 0-3 (GPIO 26-29). Currently, the connections are 

<br>

|  Team Members                 | Student ID    |
|-------------------------------|---------------|
|  Aloysius Loo Wei Jun         | 2101162       |
|  James Nicolas Tan Cher Wei   | 2101209       |
|  Jaden Edwin Chua             | 1802965       |

<br>

## Description
```md
Our team is tasked to design and develop drivers for these two types of IR sensors. 
    1. Infrared Sensor
    2. Rotary Encoder
  • Identify, compare and validate various filter algorithms for these sensors.
  • Encoder is used to measure wheel velocity.
  • IR sensor on the ‘belly’ of the car to read barcode located on the floor.
  • Using common barcode "Type 39"
```

## Usage
### Setup Guide (Installation)
```md
1. In order to use this makefile you will need to make sure that the following
dependencies are installed on your system:
    - Visual Studio
    - Pico Installation, 
click here to [view](https://www.hackster.io/lawrence-wiznet-io/how-to-setup-raspberry-pi-pico-c-c-sdk-in-window10-f2b816)

Use Developer Command Prompt.
- Ensure button of Pico is pressed as the USB is slotted into the PC
- From '\build', put barcode.uf2 into Pico Storage
Connect to Serial Console (PuTTY) to view results
```
## Charts and Graphs

Plot Chart Examples (Illustrating different performances):

Base Graph (Raw Voltage)

<img src="\Infrared\Images\Voltage1.png">

Normalized Graph (Normalized Voltage)

<img src="\Infrared\Images\voltage2.png">

