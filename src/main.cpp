#include <CumulocityClient.h>
#ifdef ESP32
#include <WiFiClientSecure.h>
#else //ESP8266
#include <ESP8266WiFi.h>
#endif

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Configuration.h"
#endif

#include "DHT.h"
#define DHTPIN 26
#define DHTTYPE DHT21 // AM2301
DHT dht(DHTPIN, DHTTYPE);

#define FORMAT_SPIFFS_IF_FAILED true

WiFiClientSecure _wifiClient = WiFiClientSecure();
CumulocityClient _client = CumulocityClient(_wifiClient, (char *)"00000000");

void connectCumulocityServer(Settings settings, bool requestDeviceCreds)
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
  Settings settings = _config.getSettings(SPIFFS, Serial);

  // Connect to Wifi
  WiFi.begin(settings.wifiSsid, settings.wifiPassword);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  // Connect to Cumulocity
  connectCumulocityServer(settings, !_config.isPersisted(SPIFFS));
  if (!_config.isPersisted(SPIFFS))
  {
    Credentials received = _client.getCredentials();
    settings.userName = received.username;
    settings.password = received.password;
    settings.tenantId = received.tenant;
    _config.persistSettings(SPIFFS, settings);
  }

  // Init temp/humidity sensor library
  dht.begin();
}

void loop()
{
  delay(1000);
  _client.loop();

  float temperature = dht.readTemperature();
  char tempStr[10];
  sprintf(tempStr, "%f", temperature);
  _client.createMeasurement("c8y_Temperature", "T", tempStr, (char *)"*C");

  float humidity = dht.readHumidity();
  char humidityStr[10];
  sprintf(humidityStr, "%f", humidity);
  _client.createMeasurement("Humidity", "H", humidityStr, (char *)"%\t");

  int8_t rssi = WiFi.RSSI();
  char rssiStr[10];
  sprintf(rssiStr, "%d", rssi);
  _client.createMeasurement("c8y_SignalStrength", "rssi", rssiStr, (char *)"dBm");
}