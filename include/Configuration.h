#ifndef ARDUINO_H
    #define ARDUINO_H
    #include <Arduino.h>
#endif

struct Settings_t
{
    char* wifiSsid;
    char* wifiPassword;
    char* hostName;
    char* tenantId;
    char* userName;
    char* password;
    char* clientId;
};

class Configuration
{
public:
    Settings_t getSettings();
};
