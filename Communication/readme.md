## Members

Github Handle | Student ID 
--- | --- |
<a href="https://github.com/yilinTay1/">Yi Lin Tay</a> | 2103154 
<a href="https://github.com/luazhizhan/">Lua Zhi Zhan</a> | 2101393 
<a href="https://github.com/Kenny-DevTech/">Kenny Lim</a> | 2102764 

## M5StickC PLUS
M5StickC Plus Quick Start: https://docs.m5stack.com/en/quick_start/m5stickc_plus/uiflow

Only `Ground`, `G32`, and `G33` pins are required to connect to `Ground`, `P3.2`, and `P3.3` respectively.
`Pico_M5Stick_UART/uart.m5f` can be loaded and viewed on https://flow.m5stack.com/. A `MQTT client` created would publish topic- `msp432` to a local MQTT broker- Mosquitto and a UART connection to read data from `msp432`.

## Projects
### Pico_M5Stick_UART 
1. Send data from 'Pico' to `M5StickC PLUS` and to `MQTT client`.
2. Repeating timer is created to send data to m5stick at 1s interval.

### MSP_M5Stick_UART
1. Data would be send from `MSP` to `M5StickC PLUS` and to `MQTT client`
2. `TimerA` is created to send data to m5stick at a 1s interval.
3. Data would be received from `M5StickC PLUS`to `MSP` from `MQTT client`
4. `EUSCIA2 ISR` is used to handle UART A2 interrupt

### Pico_HC05_UART
Using pins 0 and 1 