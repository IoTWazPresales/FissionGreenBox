#ifndef SensorRTC_h
#define SensorRTC_h

#include <string>
#include <IoTMqttSettingsService.h>
#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#include <Adafruit_BusIO_Register.h>
#include <RTClib.h>

class RTCState {
 public:
  String timestamp;

  static void read(RTCState& settings, JsonObject& root) {
    root["timestamp"] = settings.timestamp;
  }

  static StateUpdateResult update(JsonObject& root, RTCState& state) {
    String timestamp = root["timestamp"];
    state.timestamp = timestamp;
    return StateUpdateResult::CHANGED;
  }
};

class SensorRTC : public StatefulService<RTCState> {
 public:
  SensorRTC(AsyncWebServer* server,
            SecurityManager* securityManager,
            AsyncMqttClient* mqttClient);
  void begin();
  void loop();
  void readSensor();

 private:
  HttpEndpoint<RTCState> _httpEndpoint;
  MqttPubSub<RTCState> _mqttPubSub;
  WebSocketTxRx<RTCState> _webSocket;

  RTC_DS3231 _rtc;

  unsigned long _lastReading = 0;

  void onConfigUpdated();
};

#endif
