#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif
#include "FS.h"
#include "SPIFFS.h"

struct Settings_t
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
    Settings_t readFromSerial(HardwareSerial &serial);
    Settings_t readFromFileSystem(fs::FS &fs);
public:
    Settings_t getSettings(fs::FS &fs, HardwareSerial &serial);
    bool isPersisted(fs::FS &fs);
    void persistSettings(fs::FS &fs, Settings_t settings);
};
