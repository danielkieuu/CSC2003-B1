import { useMqttState, useSubscription } from 'mqtt-react-hooks'
import type { NextPage } from 'next'
import { FormEvent, useState } from 'react'

const Home: NextPage = () => {
  const { client, connectionStatus } = useMqttState()
  const { message } = useSubscription(['embedded'])
  const [xy, setXY] = useState({ x: 0, y: 0 })

  const submit = (e: FormEvent): void => {
    e.preventDefault()
    if (!client) return
    client.publish('Dashboard', `${xy.x}${xy.y}`)
  }

  return (
    <div className="grid gap-1">
      <nav className="flex h-11 w-full justify-self-center bg-blue-600">
        <h1 className=" self-center pl-2 font-medium text-white">
          CSC2003 Group B1 Dashboard
        </h1>
      </nav>

      <div className="w-full bg-purple-200 text-center">
        <h3 className="font-medium">MQTT Connection status</h3>
        <span>{connectionStatus.toString()}</span>
      </div>

      <div className="w-full bg-orange-200 text-center">
        <h3 className="font-medium">Speed</h3>
        <span>
          {message?.message && (message.message as string).split(',')[0]}
        </span>
      </div>

      <div className="w-full bg-rose-200 text-center">
        <h3 className="font-medium">Hump Height Data</h3>
        <span>
          {message?.message && (message.message as string).split(',')[1]}
        </span>
      </div>

      <div className="w-full bg-green-200 text-center">
        <h3 className="font-medium">Distance Travelled</h3>
        <span>
          {message?.message && (message.message as string).split(',')[2]} meter
        </span>
      </div>

      <div className="w-full bg-indigo-200 text-center">
        <h3 className="font-medium">Barcode label</h3>
        <span>
          {message?.message && (message.message as string).split(',')[3]}
        </span>
      </div>

      <form
        onSubmit={submit}
        className="flex flex-col items-center justify-center"
      >
        <div className="mb-3">
          <label className="text-gray-90 mb-2 block text-sm font-medium">
            X Coordinate
          </label>
          <input
            type="text"
            inputMode="numeric"
            onChange={(e) => setXY({ ...xy, x: Number(e.target.value) })}
            className="block w-full rounded-lg border border-gray-300 bg-gray-50 p-2 text-sm text-gray-900 focus:border-blue-500 focus:ring-blue-500"
          ></input>
        </div>
        <div className="mb-3">
          <label className="text-gray-90 mb-2 block text-sm font-medium">
            Y Coordinate
          </label>
          <input
            type="text"
            inputMode="numeric"
            onChange={(e) => setXY({ ...xy, y: Number(e.target.value) })}
            className="block w-full rounded-lg border border-gray-300 bg-gray-50 p-2 text-sm text-gray-900 focus:border-blue-500 focus:ring-blue-500"
          ></input>
        </div>
        <button
          type="submit"
          className="w-full rounded-lg bg-blue-700 px-5 py-2.5 text-center text-sm font-medium text-white hover:bg-blue-800 focus:outline-none focus:ring-4 focus:ring-blue-300 sm:w-auto"
        >
          Submit
        </button>
      </form>
    </div>
  )
}

export default Home
