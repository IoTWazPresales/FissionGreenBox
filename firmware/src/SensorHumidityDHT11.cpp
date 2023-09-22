#include <SensorHumidityDHT11.h>

SensorHumidityDHT11::SensorHumidityDHT11(AsyncWebServer* server,
                                         SecurityManager* securityManager,
                                         AsyncMqttClient* mqttClient) :
    _httpEndpoint(S_H_DHT11State::read,
                  S_H_DHT11State::update,
                  this,
                  server,
                  "/rest/s_H_DHT11State",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(S_H_DHT11State::read,
                S_H_DHT11State::update,
                this,
                mqttClient,
                SettingValue::format("device/#{unique_id}/dht11/state"),
                SettingValue::format("device/#{unique_id}/dht11/set")),
    _webSocket(S_H_DHT11State::read,
               S_H_DHT11State::update,
               this,
               server,
               "/ws/s_H_DHT11State",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _dht(D3, DHT11) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SensorHumidityDHT11::begin() {
  // _state.temperature = 999;
  // Serial.println(F("Init temperature sensor"));
  Serial.print("Starting DHT11 sensor...");
  _dht.begin();
  Serial.println("finished!");
  readSensor();
  onConfigUpdated();
}

void SensorHumidityDHT11::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    readSensor();
    callUpdateHandlers("sensor");
  }
}

void SensorHumidityDHT11::readSensor() {
  // Serial.println(F("Polling temperature sensors"));
  float airhumidity = _dht.readHumidity();
  float airtemperature = _dht.readTemperature();

  if (!isnan(airhumidity) && !isnan(airtemperature)) {
    Serial.print(F("DHT11 HUMIDITY: "));
    Serial.println(airhumidity);
    Serial.print(F("DHT11 TEMPERATURE: "));
    Serial.println(airtemperature);
    _state.airhumidity = airhumidity;
    _state.airtemperature = airtemperature;
  } else {
    Serial.println(F("DHT11 ERROR"));
  }
}

void SensorHumidityDHT11::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
}
