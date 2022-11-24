import type { NextPage } from 'next'

const Home: NextPage = () => {
  return (
    <div className="grid gap-1">
      <nav className="flex h-11 w-full justify-self-center bg-blue-600">
        <h1 className=" self-center pl-2 font-medium text-white">
          CSC2003 Group B1 Dashboard
        </h1>
      </nav>

      {/* Power & Voltage */}
      <h2 className="pl-2 font-medium">Power & Voltage</h2>
      <div className="flex h-full flex-col gap-1 px-1 sm:flex-row">
        <div className="w-full bg-orange-200 text-center sm:w-1/2">
          <h3 className="font-medium">Core Voltage Level</h3>
          <span>VCORE0</span>
        </div>
        <div className="w-full bg-orange-200 text-center sm:w-1/2">
          <h3 className="font-medium">Power Mode</h3>
          <span>AM_LDO_VCORE0</span>
        </div>
      </div>

      {/* Wifi */}
      <h2 className="pl-2 font-medium">Wifi</h2>
      <div className="flex h-full flex-col gap-1 px-1">
        <div className="flex flex-col gap-1 sm:flex-row">
          <div className="w-full bg-rose-200 text-center">
            <h3 className="font-medium">Reading MQTT</h3>
            <span>False</span>
          </div>
          <div className="w-full bg-rose-200 text-center">
            <h3 className="font-medium">eUSCI UART Configuration</h3>
            <span>
              SMCLK, 13, 0, 37, No Parity, LSB First, 1, UART Mode, Oversampling
            </span>
          </div>
        </div>
      </div>

      {/* Motor Control */}
      <h2 className="pl-2 font-medium">Motor Control</h2>
      <div className="flex h-full flex-col gap-1 px-1">
        <div className="flex flex-col gap-1 sm:flex-row">
          <div className="w-full bg-green-200 text-center">
            <h3 className="font-medium">Speed</h3>
            <span>10km/h</span>
          </div>
          <div className="w-full bg-green-200 text-center">
            <h3 className="font-medium">PWM Configuration</h3>
            <span>SMCLK, 5ms, RESET_SET, 10%</span>
          </div>
        </div>
        <div className="flex h-full flex-col gap-1 sm:flex-row">
          <div className="w-full bg-green-200 text-center">
            <h3 className="font-medium">Left Motor Direction</h3>
            <span>Clockwise</span>
          </div>
          <div className="w-full bg-green-200 text-center">
            <h3 className="font-medium">Right Motor Direction</h3>
            <span>Clockwise</span>
          </div>
        </div>
      </div>

      {/* Ultrasonic */}
      <h2 className="pl-2 font-medium">Ultrasonic</h2>
      <div className="flex h-full flex-col gap-1 px-1">
        <div className="w-full bg-purple-200 text-center">
          <h3 className="font-medium">Up Mode Configuration</h3>
          <span>SMCLK, 1000 Tick Period, False, False, Clear</span>
        </div>
        <div className="flex flex-col gap-1 sm:flex-row">
          <div className="w-full bg-purple-200 text-center">
            <h3 className="font-medium">Front Distance</h3>
            <span>10cm</span>
          </div>
          <div className="w-full bg-purple-200 text-center">
            <h3 className="font-medium">Back Distance</h3>
            <span>5cm</span>
          </div>
        </div>
        <div className="flex flex-col gap-1 sm:flex-row">
          <div className="w-full bg-purple-200 text-center">
            <h3 className="font-medium">Left Distance</h3>
            <span>15cm</span>
          </div>
          <div className="w-full bg-purple-200 text-center">
            <h3 className="font-medium">Right Distance</h3>
            <span>3cm</span>
          </div>
        </div>
      </div>

      {/* Mapping */}
      <h2 className="pl-2 font-medium">Mapping</h2>
    </div>
  )
}

export default Home
