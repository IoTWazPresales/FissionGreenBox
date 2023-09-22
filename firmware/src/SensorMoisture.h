#ifndef SensorMoisture_h
#define SensorMoisture_h

#include <IoTMqttSettingsService.h>
#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

class MoistureState {
 public:
  float moisture;

  static void read(MoistureState& settings, JsonObject& root) {
    root["moisture"] = settings.moisture;
  }

  static StateUpdateResult update(JsonObject& root, MoistureState& state) {
    state.moisture = root["moisture"];
    return StateUpdateResult::CHANGED;
  }
};

class SensorMoisture : public StatefulService<MoistureState> {
 public:
  SensorMoisture(
    AsyncWebServer* server, 
    SecurityManager* securityManager, 
    AsyncMqttClient* mqttClient
  );
  
  void begin();
  void loop();
  void readSensor();

 private:
  HttpEndpoint<MoistureState> _httpEndpoint;
  MqttPubSub<MoistureState> _mqttPubSub;
  WebSocketTxRx<MoistureState> _webSocket;

  unsigned long _lastReading = 0;

  void onConfigUpdated();
};

#endif
