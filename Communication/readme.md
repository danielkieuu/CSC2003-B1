## Members

Github Handle | Student ID 
--- | --- |
<a href="https://github.com/yilinTay1/">Yi Lin Tay</a> | 2103154 
<a href="https://github.com/luazhizhan/">Lua Zhi Zhan</a> | 2101393 
<a href="https://github.com/Kenny-DevTech/">Kenny Lim</a> | 2102764 

## Pico Set-up
There are a few option listed to setup Pico:
1. Reference this Video Tutorial [Click Here](https://www.youtube.com/watch?v=B5rQSoOmR5w) 
2. Reference this Written Guide [Click Here](https://www.digikey.sg/en/maker/projects/raspberry-pi-pico-and-rp2040-cc-part-1-blink-and-vs-code/7102fb8bca95452e9df6150f39ae8422)
3. Reference this Official Guide, Chapter 9.2 for guide on setting up on Windows [Click Here](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) 

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

## Dashboard
This is a [Next.js](https://nextjs.org/) project bootstrapped with [`create-next-app`](https://github.com/vercel/next.js/tree/canary/packages/create-next-app).

### Getting Started
Run the development server:
```bash
npm run dev
# or
yarn dev
```
Open [http://localhost:3000](http://localhost:3000) with your browser to see the result.
You can start editing the page by modifying `pages/index.tsx`. The page auto-updates as you edit the file.

[API routes](https://nextjs.org/docs/api-routes/introduction) can be accessed on [http://localhost:3000/api/hello](http://localhost:3000/api/hello). This endpoint can be edited in `pages/api/hello.ts`.

The `pages/api` directory is mapped to `/api/*`. Files in this directory are treated as [API routes](https://nextjs.org/docs/api-routes/introduction) instead of React pages.

### Learn More
To learn more about Next.js, take a look at the following resources:

- [Next.js Documentation](https://nextjs.org/docs) - learn about Next.js features and API.
- [Learn Next.js](https://nextjs.org/learn) - an interactive Next.js tutorial.

You can check out [the Next.js GitHub repository](https://github.com/vercel/next.js/) - your feedback and contributions are welcome!

### Deploy on Vercel
The easiest way to deploy your Next.js app is to use the [Vercel Platform](https://vercel.com/new?utm_medium=default-template&filter=next.js&utm_source=create-next-app&utm_campaign=create-next-app-readme) from the creators of Next.js.

Check out our [Next.js deployment documentation](https://nextjs.org/docs/deployment) for more details.

