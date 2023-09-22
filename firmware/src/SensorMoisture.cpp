#include <SensorMoisture.h>

SensorMoisture::SensorMoisture(AsyncWebServer* server, SecurityManager* securityManager, AsyncMqttClient* mqttClient) :
    _httpEndpoint(MoistureState::read,
                  MoistureState::update,
                  this,
                  server,
                  "/rest/MoistureState",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(MoistureState::read,
                MoistureState::update,
                this,
                mqttClient,
                SettingValue::format("device/#{unique_id}/soil/state"),
                SettingValue::format("device/#{unique_id}/soil/set")),
    _webSocket(MoistureState::read,
               MoistureState::update,
               this,
               server,
               "/ws/MoistureState",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SensorMoisture::begin() {
  // _state.temperature = 999;
  // Serial.println(F("Init temperature sensor"));
  Serial.print("Starting Moisture sensor...");
  readSensor();
  onConfigUpdated();
}

void SensorMoisture::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    readSensor();
    callUpdateHandlers("sensor");
  }
}

void SensorMoisture::readSensor() {
  float moisture = (4095.0 - analogRead(A0)) / 40.95;  // convert to %
  Serial.print(F("MOISTURE: "));
  Serial.println(moisture);
  _state.moisture = moisture;
}

void SensorMoisture::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
}
