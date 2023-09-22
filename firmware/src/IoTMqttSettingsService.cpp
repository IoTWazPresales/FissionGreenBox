#include <IoTMqttSettingsService.h>

IoTMqttSettingsService::IoTMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(IoTMqttSettings::read,
                  IoTMqttSettings::update,
                  this,
                  server,
                  LIGHT_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(IoTMqttSettings::read, IoTMqttSettings::update, this, fs, LIGHT_BROKER_SETTINGS_FILE) {
}

void IoTMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
