#include <CumulocityClient.h>
#ifdef ESP32
#include <WiFiClientSecure.h>
#else //ESP8266
#include <ESP8266WiFi.h>
#endif
#include "Credentials.h"

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Configuration.h"
#endif

WiFiClientSecure _wifiClient = WiFiClientSecure();
CumulocityClient _client = CumulocityClient(_wifiClient, "00000000");

void setup()
{
  Serial.begin(115200);

  Configuration _config;
  Settings_t settings = _config.getSettings();

  WiFi.begin(settings.wifiSsid, settings.wifiPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  _client.setDeviceId(settings.clientId);
  _client.connect(settings.hostName, 8883, settings.tenantId, settings.userName, settings.password);
  _client.registerDevice(settings.clientId, "c8y_esp32");
}

void loop()
{
  delay(1000);
  _client.loop();
  _client.createMeasurement("Temperature", "T", "20.5", "*C");

  int8_t rssi = WiFi.RSSI();
  char rssiStr[10];
  sprintf(rssiStr, "%d", rssi);
  _client.createMeasurement("SignalStrength", "T", rssiStr, "*m");
}