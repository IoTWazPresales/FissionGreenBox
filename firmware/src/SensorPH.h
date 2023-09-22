#ifndef SensorPH_h
#define SensorPH_h

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>
#include <IoTMqttSettingsService.h>

class PHState {
 public:
  float ph;

  static void read(PHState& settings, JsonObject& root) {
    root["ph"] = settings.ph;
  }

  static StateUpdateResult update(JsonObject& root, PHState& state) {
    state.ph = root["ph"];
    return StateUpdateResult::CHANGED;
  }
};

class SensorPH : public StatefulService<PHState> {
 public:
  SensorPH(AsyncWebServer* server,
           SecurityManager* securityManager,
           AsyncMqttClient* mqttClient);
           
  void begin();
  void loop();
  void readSensor();

 private:
  HttpEndpoint<PHState> _httpEndpoint;
  MqttPubSub<PHState> _mqttPubSub;
  WebSocketTxRx<PHState> _webSocket;

  unsigned long _lastReading = 0;

  void onConfigUpdated();
};

#endif
