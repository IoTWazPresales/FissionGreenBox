#ifndef SensorRelay_h
#define SensorRelay_h

#include <IoTMqttSettingsService.h>
#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define RELAY_CH1 17
#define RELAY_CH2 16
#define RELAY_CH3 4
#define RELAY_CH4 12

class RelayState {
 public:
  bool ch1;
  bool ch2;
  bool ch3;
  bool ch4;

  static void read(RelayState& settings, JsonObject& root) {
    root["ch1"] = settings.ch1;
    root["ch2"] = settings.ch2;
    root["ch3"] = settings.ch3;
    root["ch4"] = settings.ch4;
  }

  static StateUpdateResult update(JsonObject& root, RelayState& state) {
    Serial.println("State Update!");
    state.ch1 = root["ch1"];
    state.ch2 = root["ch2"];
    state.ch3 = root["ch3"];
    state.ch4 = root["ch4"];
    return StateUpdateResult::CHANGED;
  }

};

class SensorRelay : public StatefulService<RelayState> {
 public:
  
  SensorRelay(
    AsyncWebServer* server,
    SecurityManager* securityManager,
    AsyncMqttClient* mqttClient
  );

  void begin();
  void loop();

 private:
  HttpEndpoint<RelayState> _httpEndpoint;
  MqttPubSub<RelayState> _mqttPubSub;
  WebSocketTxRx<RelayState> _webSocket;

  unsigned long _lastReading = 0;

  void onConfigUpdated();
};

#endif
