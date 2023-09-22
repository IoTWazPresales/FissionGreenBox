#include <SensorTDS.h>

SensorTDS::SensorTDS(AsyncWebServer* server, SecurityManager* securityManager, AsyncMqttClient* mqttClient) :
    _httpEndpoint(TDSState::read,
                  TDSState::update,
                  this,
                  server,
                  "/rest/TDSState",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(TDSState::read,
                TDSState::update,
                this,
                mqttClient,
                SettingValue::format("device/#{unique_id}/tds/state"),
                SettingValue::format("device/#{unique_id}/tds/set")),
    _webSocket(TDSState::read,
               TDSState::update,
               this,
               server,
               "/ws/TDSState",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SensorTDS::begin() {
  // _state.temperature = 999;
  // Serial.println(F("Init temperature sensor"));
  Serial.print("Starting TDS sensor...");
  readSensor();
  onConfigUpdated();
}

void SensorTDS::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    readSensor();
    callUpdateHandlers("sensor");
  }
}

void SensorTDS::readSensor() {
  float tds = analogRead(A1) / 40.95;  // convert to %
  Serial.print(F("TDS: "));
  Serial.println(tds);
  _state.tds = tds;
}

void SensorTDS::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
}
