#ifndef TemperatureStateService_h
#define TemperatureStateService_h

#include <IoTMqttSettingsService.h>
#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureState {
 public:
  float watertemperature;

  static void read(TemperatureState& settings, JsonObject& root) {
    root["watertemperature"] = settings.watertemperature;
  }

  static StateUpdateResult update(JsonObject& root, TemperatureState& state) {
    float watertemperature = root["watertemperature"];
    state.watertemperature = watertemperature;
    return StateUpdateResult::CHANGED;
  }
};

class TemperatureStateService : public StatefulService<TemperatureState> {
 public:
  
  TemperatureStateService(
    AsyncWebServer* server,
    SecurityManager* securityManager,
    AsyncMqttClient* mqttClient
  );

  void begin();
  void loop();
  void readSensor();

 private:
  HttpEndpoint<TemperatureState> _httpEndpoint;
  MqttPubSub<TemperatureState> _mqttPubSub;
  WebSocketTxRx<TemperatureState> _webSocket;

  OneWire _ds = OneWire(D2);
  DallasTemperature _tempSensors = DallasTemperature(&_ds);
  unsigned long _lastReading = 0;

  void onConfigUpdated();
};

#endif
