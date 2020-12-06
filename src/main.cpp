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

#define FORMAT_SPIFFS_IF_FAILED true

WiFiClientSecure _wifiClient = WiFiClientSecure();
CumulocityClient _client = CumulocityClient(_wifiClient, (char *)"00000000");

void connectCumulocityServer(Settings_t settings, bool requestDeviceCreds)
{
  Serial.println("Connecting to Cumulocity...");
  _client.setDeviceId(settings.clientId);
  _client.connect(settings.hostName, 8883, settings.tenantId, settings.userName, settings.password);

  if (requestDeviceCreds)
  {
    _client.retrieveDeviceCredentials();
    while (!_client.checkCredentialsReceived())
    {
      Serial.print("#");
      delay(1000);
    }

    Serial.println("Reconnecting to Cumulocity");

    _client.disconnect();
    _client.reconnect();
  }

  Serial.println("Registering device...");
  _client.registerDevice(settings.clientId, (char *)"c8y_esp32");
}

void setup()
{
  Serial.begin(115200);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  //SPIFFS.format();

  Configuration _config;
  Settings_t settings = _config.getSettings(SPIFFS, Serial);

  WiFi.begin(settings.wifiSsid, settings.wifiPassword);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  connectCumulocityServer(settings, !_config.isPersisted(SPIFFS));

  if (!_config.isPersisted(SPIFFS))
  {
    Credentials received = _client.getCredentials();
    settings.userName = received.username;
    settings.password = received.password;
    settings.tenantId = received.tenant;
    _config.persistSettings(SPIFFS, settings);
  }
}

void loop()
{
  delay(1000);
  _client.loop();
  _client.createMeasurement("Temperature", "T", "20.5", (char *)"*C");

  int8_t rssi = WiFi.RSSI();
  char rssiStr[10];
  sprintf(rssiStr, "%d", rssi);
  _client.createMeasurement("SignalStrength", "T", rssiStr, (char *)"*m");
}