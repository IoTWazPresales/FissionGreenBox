// http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718037
// https://docs.solace.com/MQTT-311-Prtl-Conformance-Spec/MQTT%20Control%20Packet%20format.htm#_Toc430864887

// todo check mqtt protocol version 3 or 4

import { EventEmitter } from "events"
import * as net from "net";
import * as tls from "tls";

import * as mqttpacket from 'mqtt-packet'

import { generateDifficult, isJson } from "./utils";
import { Logger } from "./log";


interface MQTTClient {
    clientId: string
    socket: net.Socket
    subscriptions: string[]
}

export class MQTTServer extends EventEmitter {
    mqttPort = 1883;
    server: net.Server;
    connectedClients: MQTTClient[] = [];

    constructor() {
        super();
        this.server = net.createServer(this.handleClient);
        this.server.listen(this.mqttPort);
    }

    log = new Logger(this).log;

    handleClient = (socket: net.Socket) => {
        const opts = { protocolVersion: 4 }; // default is 4. Usually, opts is a connect packet
        const parser = mqttpacket.parser(opts);

        let clientId = 'not set';
        this.log('mqtt new socket client');

        socket.on('data', (data) => {
            parser.parse(data);
        })

        socket.on("close", (had_error) => {
            this.log(`mqtt client close had_error: ${had_error}`);
            socket.end();
            socket.destroy()
            //clearInterval(pinger);
            this.removeClient(clientId);
        })

        socket.on("connect", () => { this.log(`mqtt client connect`); })
        socket.on("drain", () => { this.log(`mqtt client drain`); })
        socket.on("end", () => { this.log(`mqtt client end`); });
        socket.on("error", (err) => { this.log(`mqtt client error: ${err.message}`); })
        socket.on("timeout", () => { this.log(`mqtt client timeout`); })
        socket.on("lookup", (err, address, family, host) => {
            this.log(`mqtt client lookup ${{ err, address, family, host }}`);
        })

        let authed = false;
        // TODO
        let authed_user: any = null;
        // TODO
        let last_packet: any = undefined

        // let pinger = setInterval(() => {
        //     if (authed_user) {
        //         this.log(`mqtt ping client ${id}`)
        //         socket.write(mqttpacket.generate({
        //             cmd: 'pingreq'
        //         }))
        //     }
        // }, 5000)

        parser.on('packet', async packet => {
            // console.log(packet)

            // handle connections
            if (packet.cmd == "connect") {
                this.log("New client connected", packet)

                clientId = packet.clientId;

                let client: MQTTClient = {
                    clientId: packet.clientId,
                    socket,
                    subscriptions: [],
                }

                // keep track of client
                this.addClient(client);

                socket.write(mqttpacket.generate({
                    cmd: 'connack',
                    returnCode: 0,
                    sessionPresent: true
                }));
            }
            // end of connect auth

            // if (!authed) { return; } // MUST BE AUTHENTICATED TO DO ANYTHING ELSE
            // if (!authed_user) { return; }
            // if (!authed_user.apikey) { return; }



            //// - - - - -
            if (packet.cmd == "subscribe") {

                this.addSubscriptionsToUser(clientId, packet.subscriptions.map( s => { return s.topic }));

                for (var sub of packet.subscriptions) {
                    socket.write(mqttpacket.generate({
                        cmd: 'suback',
                        messageId: packet.messageId,
                        granted: [0]
                    }));
                }
            }
            //// - - - - -
            if (packet.cmd == "pingreq") {
                // console.log('mqtt ping resp')
                // updates last seen and so on so we know the device is alive.
                if (last_packet) {
                    if (last_packet.id) {
                        let heartbeat = { id: last_packet.id, userid: authed_user.userid };
                        console.log("PING REQ");
                        // await this.props.handleCorePacket(authed_user.apikey[0], heartbeat)
                    }
                }

                socket.write(mqttpacket.generate({
                    cmd: 'pingresp'
                }))
            }

            if (packet.cmd == "publish") {

                console.log(packet.topic, packet.payload.toString())

                this.broadcast(clientId, packet.topic, JSON.parse(packet.payload.toString()))

                if (packet.qos > 0) {
                    socket.write(mqttpacket.generate({
                        cmd: 'puback',
                        messageId: packet.messageId
                    }))
                }

            }

        })

    }

    /**
     * sends out packet to subscribed clients
     */
    broadcast(clientId: string, topic: string, packet: any) {
        for (let client of this.connectedClients) {

            if (client.clientId !== clientId) {
                for (let sub of client.subscriptions) {


                    
                    if (topic === sub) {
                        let cleanpacket: any = packet;
                        delete cleanpacket._id;

                        


                        this.log(`broadcasting to ${client.clientId} on ${topic} `)

                        client.socket.write(mqttpacket.generate({
                            cmd: 'publish',
                            topic,
                            payload: Buffer.from(JSON.stringify(cleanpacket)),
                            qos: 0,
                            dup: false,
                            retain: false
                        }));

                    }
                }
            }


        }
    }

    addClient(client: MQTTClient) {
        this.log('addClient', client.clientId)
        let found = false;
        for (let c of this.connectedClients) {
            if (c.clientId === client.clientId) {
                found = true;
            }
        }
        if (found === false) {
            this.connectedClients.push(client);
            this.log(`mqtt adding client ${client.clientId} connected count after add: ${this.connectedClients.length}`);
        }
    }

    addSubscriptionsToUser(clientId: string, subscriptions: string[]) {
        this.log('addSubscriptionsToUser', clientId, subscriptions)
        for (let client of this.connectedClients) {
            if (client.clientId == clientId) {

                for (let topic of subscriptions) {
                    if (client.subscriptions.indexOf(topic) == -1) {
                        client.subscriptions.push(topic);
                        client.subscriptions = client.subscriptions.concat(subscriptions);
                    }
                }

                
            }
        }
    }

    removeClient = (clientId: string) => {
        let filtered = this.connectedClients.filter(i => (i.clientId != clientId))
        this.connectedClients = filtered;
        this.log(`mqtt removed client ${clientId} connected count: ${this.connectedClients.length}`);
    }
}
