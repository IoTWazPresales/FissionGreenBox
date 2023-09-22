#include <SensorWL.h>


SensorHumidityDHT11::SensorWL(AsyncWebServer* server,
                                         SecurityManager* securityManager,
                                         AsyncMqttClient* mqttClient) :
    _httpEndpoint(SensorWLState::read,
                  SensorWLState::update,
                  this,
                  server,
                  "/rest/SensorWLState",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(SensorWLState::read,
                SensorWLState::update,
                this,
                mqttClient,
                SettingValue::format("device/#{unique_id}/SensorWL/state"),
                SettingValue::format("device/#{unique_id}/SensorWL/set")),
    _webSocket(SensorWLState::read,
               SensorWLState::update,
               this,
               server,
               "/ws/SensorWLState",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _WL(D3, WL) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SensorWL::begin() {
  // _state.temperature = 999;
  // Serial.println(F("Init temperature sensor"));
  Serial.print("Starting Water level sensor...");
  _dht.begin();
  Serial.println("finished!");
  readSensor();
  onConfigUpdated();
}

void SensorWL::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    readSensor();
    callUpdateHandlers("sensor");
  }
}

void SensorWL::readSensor() {
  // Serial.println(F("Polling temperature sensors"));
  float waterlevel = _WL.readLevel();
  

  if (!isnan(waterlevel)) {
    Serial.print(F("Water level: "));
    Serial.println(waterlevel);
       _state.waterlevel = waterlevel;
    
  } else {
    Serial.println(F("WaterLevel Sensor Error"));
  }
}

void SensorWL::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
}
