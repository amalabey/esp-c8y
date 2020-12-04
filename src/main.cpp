#include <CumulocityClient.h>
#ifdef ESP32
#include <WiFiClientSecure.h>
#else //ESP8266
#include <ESP8266WiFi.h>
#endif
#include "Credentials.h"

const char *ssid = WIFI_SSID;
const char *wifiPassword = WIFI_PWD;
char *host = C8Y_HOST;
char *username = C8Y_USERNAME;    
char *c8yPassword = C8Y_PASSWORD; 
char *tenant = C8Y_TENANT;  
char *clientId = C8Y_CLIENTID;
const char* root_ca = ROOT_CA;

WiFiClientSecure wifiClient;
CumulocityClient c8yClient(wifiClient, clientId);

void setup()
{

  Serial.begin(115200);

  wifiClient.setCACert(root_ca);
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  c8yClient.connect(host, 8883, tenant, username, c8yPassword);

  c8yClient.registerDevice(clientId, "c8y_esp32");
}

void loop()
{
  delay(1000);
  c8yClient.loop();
  c8yClient.createMeasurement("Temperature", "T", "20.5", "*C");
  
  int8_t rssi = WiFi.RSSI();
  char rssiStr[10];
  sprintf(rssiStr, "%d", rssi);
  c8yClient.createMeasurement("SignalStrength", "T", rssiStr, "*m");
}