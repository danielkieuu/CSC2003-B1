import { Connector } from 'mqtt-react-hooks'
import type { AppProps } from 'next/app'
import '../styles/globals.css'

function MyApp({ Component, pageProps }: AppProps): JSX.Element {
  return (
    <Connector
      brokerUrl="ws://192.168.43.90:9001"
      options={{
        username: 'mqtt',
        password: 'asdqwe123',
        clean: true,
        clientId: 'web_client',
      }}
    >
      <Component {...pageProps} />
    </Connector>
  )
}

export default MyApp
