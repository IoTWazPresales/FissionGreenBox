#include <SensorRelay.h>

SensorRelay::SensorRelay(AsyncWebServer* server,
                         SecurityManager* securityManager,
                         AsyncMqttClient* mqttClient) :
    _httpEndpoint(RelayState::read,
                  RelayState::update,
                  this,
                  server,
                  "/rest/RelayState",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(RelayState::read,
                RelayState::update,
                this,
                mqttClient,
                SettingValue::format("device/#{unique_id}/relay/state"),
                SettingValue::format("device/#{unique_id}/relay/set")),
    _webSocket(RelayState::read,
               RelayState::update,
               this,
               server,
               "/ws/RelayState",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  pinMode(RELAY_CH1, OUTPUT);
  pinMode(RELAY_CH2, OUTPUT);
  pinMode(RELAY_CH3, OUTPUT);
  pinMode(RELAY_CH4, OUTPUT);
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SensorRelay::begin() {
  Serial.println("Starting Relay sensor...");
  _state.ch1 = true;
  _state.ch2 = true;
  _state.ch3 = true;
  _state.ch4 = true;
  onConfigUpdated();
}

void SensorRelay::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    callUpdateHandlers("sensor");
  }
}

void SensorRelay::onConfigUpdated() {
  Serial.print("RELAY STATE CHANGED: CH1:");
  Serial.println(_state.ch1);
  digitalWrite(RELAY_CH1, _state.ch1);
  digitalWrite(RELAY_CH2, _state.ch2);
  digitalWrite(RELAY_CH3, _state.ch3);
  digitalWrite(RELAY_CH4, _state.ch4);
}
