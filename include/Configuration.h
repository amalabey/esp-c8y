#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif
#include "FS.h"
#include "SPIFFS.h"

struct Settings
{
    char *wifiSsid;
    char *wifiPassword;
    char *hostName;
    char *tenantId;
    char *userName;
    char *password;
    char *clientId;
};

class Configuration
{
private:
    Settings readFromSerial(HardwareSerial &serial);
    Settings readFromFileSystem(fs::FS &fs);
public:
    Settings getSettings(fs::FS &fs, HardwareSerial &serial);
    bool isPersisted(fs::FS &fs);
    void persistSettings(fs::FS &fs, Settings settings);
};
