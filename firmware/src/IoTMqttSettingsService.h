#ifndef IoTMqttSettingsService_h
#define IoTMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <SettingValue.h>

#define LIGHT_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define LIGHT_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class IoTMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void read(IoTMqttSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static StateUpdateResult update(JsonObject& root, IoTMqttSettings& settings) {
    settings.mqttPath = SettingValue::format("device/#{unique_id}");
    settings.name = SettingValue::format("canna-#{unique_id}");
    settings.uniqueId = SettingValue::format("canna-#{unique_id}");
    return StateUpdateResult::CHANGED;
  }
};

class IoTMqttSettingsService : public StatefulService<IoTMqttSettings> {
 public:
  IoTMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<IoTMqttSettings> _httpEndpoint;
  FSPersistence<IoTMqttSettings> _fsPersistence;
};

#endif  // end IoTMqttSettingsService_h
