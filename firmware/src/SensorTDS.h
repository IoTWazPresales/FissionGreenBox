#ifndef SensorTDS_h
#define SensorTDS_h

#include <IoTMqttSettingsService.h>
#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

class TDSState {
 public:
  float tds;

  static void read(TDSState& settings, JsonObject& root) {
    root["tds"] = settings.tds;
  }

  static StateUpdateResult update(JsonObject& root, TDSState& state) {
    state.tds = root["tds"];
    return StateUpdateResult::CHANGED;
  }
};

class SensorTDS : public StatefulService<TDSState> {
 public:
  SensorTDS(AsyncWebServer* server,
            SecurityManager* securityManager,
            AsyncMqttClient* mqttClient);
  void begin();
  void loop();
  void readSensor();

 private:
  HttpEndpoint<TDSState> _httpEndpoint;
  MqttPubSub<TDSState> _mqttPubSub;
  WebSocketTxRx<TDSState> _webSocket;

  unsigned long _lastReading = 0;

  void onConfigUpdated();
};

#endif
