#include <SensorPH.h>

SensorPH::SensorPH(AsyncWebServer* server,
                   SecurityManager* securityManager,
                   AsyncMqttClient* mqttClient) :
    _httpEndpoint(PHState::read,
                  PHState::update,
                  this,
                  server,
                  "/rest/PHState",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(PHState::read, PHState::update, this, mqttClient,
                SettingValue::format("device/#{unique_id}/ph/state"),
                SettingValue::format("device/#{unique_id}/ph/set")),
    _webSocket(PHState::read,
               PHState::update,
               this,
               server,
               "/ws/PHState",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SensorPH::begin() {
  // _state.temperature = 999;
  // Serial.println(F("Init temperature sensor"));
  Serial.print("Starting PH sensor...");
  readSensor();
  onConfigUpdated();
}

void SensorPH::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    readSensor();
    callUpdateHandlers("sensor");
  }
}

void SensorPH::readSensor() {
  // Note: I'm not sure the sensor is reliable, or perhaps my code/circuit.
  float voltage = analogRead(A2) * 3.3 / 4096.0;  // convert to mV
  float pHValue = ((voltage - 0.97) * 7.0) + 7.0;
  Serial.print(F("PH: "));
  Serial.println(pHValue);
  _state.ph = (_state.ph * 0.9) + (pHValue * 0.1);  // smooth over 10 samples
}

void SensorPH::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
}
