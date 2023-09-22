#include <ESP8266React.h>
#include <IoTMqttSettingsService.h>
#include <LightStateService.h>

#include <TemperatureStateService.h>
#include <SensorHumidityDHT11.h>
#include <SensorMoisture.h>
#include <SensorTDS.h>
#include <SensorPH.h>
#include <SensorRTC.h>
#include <SensorRelay.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);

IoTMqttSettingsService iotMqttSettingsService =
    IoTMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());

LightStateService lightStateService =
    LightStateService(&server, esp8266React.getSecurityManager(), esp8266React.getMqttClient());

// TEMPERATURE
TemperatureStateService temperatureStateService =
    TemperatureStateService(&server, esp8266React.getSecurityManager(), esp8266React.getMqttClient());

// HUMIDITY DHT11
SensorHumidityDHT11 sensorHumidityDHT11 = SensorHumidityDHT11(&server,
                                                              esp8266React.getSecurityManager(),
                                                              esp8266React.getMqttClient());

// MOISTURE
SensorMoisture sensorMoisture =
    SensorMoisture(&server, esp8266React.getSecurityManager(), esp8266React.getMqttClient());

SensorTDS sensorTDS =
    SensorTDS(&server, esp8266React.getSecurityManager(), esp8266React.getMqttClient());

SensorPH sensorPH =
    SensorPH(&server, esp8266React.getSecurityManager(), esp8266React.getMqttClient());

SensorRTC sensorRTC =
    SensorRTC(&server, esp8266React.getSecurityManager(), esp8266React.getMqttClient());

SensorRelay sensorRelay = SensorRelay(&server, esp8266React.getSecurityManager(), esp8266React.getMqttClient());

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightStateService.begin();

  // start the light service
  iotMqttSettingsService.begin();

  // TEMPERATURE
  temperatureStateService.begin();

  // HUMIDITY DHT11
  sensorHumidityDHT11.begin();

  sensorMoisture.begin();

  sensorTDS.begin();

  sensorPH.begin();

  sensorRTC.begin();

  sensorRelay.begin();

  // start the server
  server.begin();

  // experimental
  update_handler_id_t myUpdateHandler = sensorRelay.addUpdateHandler([&](const String& originId) {
    Serial.print("The relay has been updated by: ");
    Serial.println(originId);
  });
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();
  temperatureStateService.loop();
  sensorHumidityDHT11.loop();
  sensorMoisture.loop();
  sensorTDS.loop();
  sensorPH.loop();
  sensorRTC.loop();
  sensorRelay.loop();
}
