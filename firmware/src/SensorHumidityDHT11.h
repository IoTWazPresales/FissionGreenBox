#ifndef SensorHumidityDHT11_h
#define SensorHumidityDHT11_h

#include <IoTMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

class S_H_DHT11State {
 public:
  float airtemperature;
  float airhumidity;

  static void read(S_H_DHT11State& settings, JsonObject& root) {
    root["airtemperature"] = settings.airtemperature;
    root["airhumidity"] = settings.airhumidity;
  }

  static StateUpdateResult update(JsonObject& root, S_H_DHT11State& state) {
    state.airtemperature = root["airtemperature"];
    state.airhumidity = root["airhumidity"];
    return StateUpdateResult::CHANGED;
  }
};

class SensorHumidityDHT11 : public StatefulService<S_H_DHT11State> {
 public:
  SensorHumidityDHT11(AsyncWebServer* server,
                      SecurityManager* securityManager,
                      AsyncMqttClient* mqttClient);

  void begin();
  void loop();
  void readSensor();

 private:
  HttpEndpoint<S_H_DHT11State> _httpEndpoint;
  MqttPubSub<S_H_DHT11State> _mqttPubSub;
  WebSocketTxRx<S_H_DHT11State> _webSocket;

  DHT _dht;

  unsigned long _lastReading = 0;

  void registerConfig();
  void onConfigUpdated();
};

#endif
