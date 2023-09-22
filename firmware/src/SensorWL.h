#ifndef SensorWL
#define SensorWL

#include <IoTMqttSettingsService.h>
#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

class SensorWLState {
 public:
  float WL;

  static void read(SensorWLState& settings, JsonObject& root) {
    root["WL"] = settings.tds;
  }

  static StateUpdateResult update(JsonObject& root, SensorWLState& state) {
    state.WL = root["WL"];
    return StateUpdateResult::CHANGED;
  }
};

class SensorWL : public StatefulService<SensorWLState> {
 public:
  SensorWL(AsyncWebServer* server,
            SecurityManager* securityManager,
            AsyncMqttClient* mqttClient);
  void begin();
  void loop();
  void readSensor();

 private:
  HttpEndpoint<SensorWLState> _httpEndpoint;
  MqttPubSub<SensorWLState> _mqttPubSub;
  WebSocketTxRx<SensorWLState> _webSocket;

  unsigned long _lastReading = 0;

  void onConfigUpdated();
};

#endif
