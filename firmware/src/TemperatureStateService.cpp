#include <TemperatureStateService.h>

TemperatureStateService::TemperatureStateService(AsyncWebServer* server,
                                                 SecurityManager* securityManager,
                                                 AsyncMqttClient* mqttClient) :
    _httpEndpoint(TemperatureState::read,
                  TemperatureState::update,
                  this,
                  server,
                  "/rest/TemperatureState",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(TemperatureState::read,
                TemperatureState::update,
                this,
                mqttClient,
                SettingValue::format("device/#{unique_id}/ds18b20/state"),
                SettingValue::format("device/#{unique_id}/ds18b20/set")),
    _webSocket(TemperatureState::read,
               TemperatureState::update,
               this,
               server,
               "/ws/TemperatureState",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void TemperatureStateService::begin() {
  Serial.print("Starting temp sensor bus...");
  readSensor();

  

  onConfigUpdated();
}

void TemperatureStateService::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    readSensor();
    callUpdateHandlers("sensor");
  }
}

void TemperatureStateService::readSensor() {
  _tempSensors.begin();
  _tempSensors.setResolution(12);
  _tempSensors.setWaitForConversion(true);

  Serial.print("Found ");
  Serial.print(_tempSensors.getDS18Count());
  Serial.println(" sensor(s).");

  // Serial.println(F("Polling temperature sensors"));
  _tempSensors.requestTemperatures();
  // Serial.println(F("DONE"));
  float tempC = _tempSensors.getTempCByIndex(0);
  // Check if reading was successful
  if (tempC != DEVICE_DISCONNECTED_C) {
    // Serial.print("Temperature for the device 1 (index 0) is: ");
    // Serial.println(tempC);
    _state.watertemperature = tempC;
    Serial.print(F("DS18B20 TEMPERATURE: "));
    Serial.println(tempC);
  } else {
    Serial.println("DS18B20 ERROR");
  }
}

void TemperatureStateService::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
}
