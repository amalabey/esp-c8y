#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Configuration.h"
#endif

#include "Credentials.h"

Settings_t Configuration::getSettings()
{
    Settings_t settings;
    settings.wifiSsid = (char*)WIFI_SSID;
    settings.wifiPassword = (char*)WIFI_PWD;
    settings.hostName = (char*)C8Y_HOST;
    settings.tenantId = (char*)C8Y_TENANT;
    settings.userName = (char*)C8Y_USERNAME;
    settings.password = (char*)C8Y_PASSWORD;
    settings.clientId = (char*)C8Y_CLIENTID;
    return settings;
}