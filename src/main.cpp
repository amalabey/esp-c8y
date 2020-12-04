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
char *username = C8Y_USERNAME;    // fixed credentials can be registered in the Administration section
char *c8yPassword = C8Y_PASSWORD; // create a user in usermanagement with the "device"role and fill the credentials here
char *tenant = C8Y_TENANT;      //tenant ID can be found by clicking on your name in the top right corner of Cumulocity
char *clientId = C8Y_CLIENTID;   //Should be a unique identifier for this device, e.g. IMEI, MAC address or SerialNumber
//uint64_t chipid = ESP.getEfuseMac();
//static const char *fingerprint PROGMEM = "28 66 9c 6a 11 13 0f 7f 7f c5 29 f8 67 e1 30 5f 13 a8 2b 21";
const char* root_ca = ROOT_CA;


WiFiClientSecure wifiClient;
CumulocityClient c8yClient(wifiClient, clientId);

void setup()
{

  Serial.begin(115200);
  //wifiClient.setFingerprint(fingerprint);
  wifiClient.setCACert(root_ca);
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  c8yClient.connect(host, tenant, username, c8yPassword);

  c8yClient.registerDevice(clientId, "c8y_esp32");
}

void loop()
{
  delay(1000);
  c8yClient.loop();
  c8yClient.createMeasurement("Temperature", "T", "20.5", "*C");
}