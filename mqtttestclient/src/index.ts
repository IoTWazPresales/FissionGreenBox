
import mqtt from 'mqtt'

const client = mqtt.connect('mqtt://localhost:1883')

client.on('connect', function () {
    client.subscribe('device/e0e2e670199c/relay/state', function (err) {
        if (!err) {
            client.publish('device/e0e2e670199c/relay/set', JSON.stringify({
                ch1: false,
                ch2: false,
                ch3: false,
                ch4: true
            }))
        }
    })
})

client.on('message', function (topic, message) {
    // message is Buffer
    console.log(message.toString())
    // client.end()
})