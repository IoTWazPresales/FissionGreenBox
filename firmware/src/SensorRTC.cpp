#include <SensorRTC.h>

// https://registry.platformio.org/libraries/adafruit/RTClib/examples/ds3231/ds3231.ino

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

SensorRTC::SensorRTC(AsyncWebServer* server, SecurityManager* securityManager, AsyncMqttClient* mqttClient) :
    _httpEndpoint(RTCState::read,
                  RTCState::update,
                  this,
                  server,
                  "/rest/RTCState",
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(RTCState::read,
                RTCState::update,
                this,
                mqttClient,
                SettingValue::format("device/#{unique_id}/rtc/state"),
                SettingValue::format("device/#{unique_id}/rtc/set")),
    _webSocket(RTCState::read,
               RTCState::update,
               this,
               server,
               "/ws/RTCState",
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SensorRTC::begin() {
  // _state.temperature = 999;
  // Serial.println(F("Init temperature sensor"));
  Serial.print("Starting RTC sensor...");
  if (!_rtc.begin()) {
    Serial.print(F("RTC ERROR"));
  }

  if (_rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  readSensor();
  onConfigUpdated();
}

void SensorRTC::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);

  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;
    readSensor();
    callUpdateHandlers("sensor");
  }
}

void SensorRTC::readSensor() {
  DateTime now = _rtc.now();
  String timestamp = now.timestamp();
  // Serial.print(now.year(), DEC);
  // Serial.print('/');
  // Serial.print(now.month(), DEC);
  // Serial.print('/');
  // Serial.print(now.day(), DEC);
  // Serial.print(" (");
  // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  // Serial.print(") ");
  // Serial.print(now.hour(), DEC);
  // Serial.print(':');
  // Serial.print(now.minute(), DEC);
  // Serial.print(':');
  // Serial.print(now.second(), DEC);
  // Serial.println();

  // Serial.print(" since midnight 1/1/1970 = ");
  // Serial.print(now.unixtime());
  // Serial.print("s = ");
  // Serial.print(now.unixtime() / 86400L);
  // Serial.println("d");

  _state.timestamp = timestamp;  //.c_str();
  Serial.print(F("RTC: "));
  Serial.println(_state.timestamp);
}

void SensorRTC::onConfigUpdated() {
  // digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
}
