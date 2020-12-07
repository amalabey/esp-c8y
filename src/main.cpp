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
char* _wifiSsid;
char* _wifiPassword;
char* _hostName;
char* _tenantId;
char* _userName;
char* _password;
char* _clientId;

void printConfig()
{
  Serial.printf("WifiSsid: %s \n", _wifiSsid);
  Serial.printf("Wifi Psk: %s \n", _wifiPassword);
  Serial.printf("Hostname: %s \n", _hostName);
  Serial.printf("Tenant Id: %s \n", _tenantId);
  Serial.printf("Username: %s \n", _userName);
  Serial.printf("Password: %s \n", _password);
  Serial.printf("ClientId: %s \n", _clientId);
}

void connectCumulocityServer(bool requestDeviceCreds)
{
  Serial.println("Connecting to Cumulocity...");
  
  _client.setDeviceId(_clientId);
  _client.connect(_hostName, 8883, _tenantId, _userName, _password);

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
  _client.registerDevice(_clientId, (char *)"c8y_esp32");
}

void setup()
{
  Serial.begin(115200);

  // Initialize file system to store configuration
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  SPIFFS.format();

  // Load the configuration
  Configuration _config;
  Settings settings = _config.getSettings(SPIFFS, Serial);
  _wifiSsid = strdup(settings.wifiSsid.c_str());
  _wifiPassword = strdup(settings.wifiPassword.c_str());
  _hostName = strdup(settings.hostName.c_str());
  _tenantId = strdup(settings.tenantId.c_str());
  _userName = strdup(settings.userName.c_str());
  _password = strdup(settings.password.c_str());
  _clientId = strdup(settings.clientId.c_str());
  printConfig();

  // Connect to Wifi
  WiFi.begin(_wifiSsid, _wifiPassword);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  // Connect to Cumulocity
  connectCumulocityServer(!_config.isPersisted(SPIFFS));
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