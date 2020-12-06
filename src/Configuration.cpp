#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Configuration.h"
#endif
#include <string.h>

#include "Credentials.h"

const char *CONFIG_FILE_NAME = "settings";

Settings_t Configuration::getSettings()
{
    Settings_t settings;
    settings.wifiSsid = (char *)WIFI_SSID;
    settings.wifiPassword = (char *)WIFI_PWD;
    settings.hostName = (char *)C8Y_HOST;
    settings.tenantId = (char *)C8Y_TENANT;
    settings.userName = (char *)C8Y_USERNAME;
    settings.password = (char *)C8Y_PASSWORD;
    settings.clientId = (char *)C8Y_CLIENTID;
    return settings;
}

void Configuration::writeToFileSystem(fs::FS &fs, Settings_t settings)
{
    Serial.printf("Writing file: %s\r\n", CONFIG_FILE_NAME);

    File file = fs.open(CONFIG_FILE_NAME, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    String message;
    message += settings.wifiSsid + '\n';
    message += settings.wifiPassword + '\n';
    message += settings.hostName + '\n';
    message += settings.tenantId + '\n';
    message += settings.userName + '\n';
    message += settings.password + '\n';
    message += settings.clientId + '\n';

    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}


Settings_t Configuration::readFromFileSystem(fs::FS &fs)
{
    Serial.printf("Reading file: %s\r\n", CONFIG_FILE_NAME);

    char buffer[4096];
    
    File file = fs.open(CONFIG_FILE_NAME, FILE_READ);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to read the file");
        throw(-1);
    }

    int length = 0;
    while (file.available())
    {
        char c = file.read();
        buffer[length++] = c;
    }
    buffer[length] = '\0';

    Settings_t settings;
    settings.wifiSsid = strtok(buffer, "\n");
    settings.wifiPassword = strtok(NULL, "\n");
    settings.hostName = strtok(NULL, "\n");
    settings.tenantId = strtok(NULL, "\n");
    settings.userName = strtok(NULL, "\n");
    settings.password = strtok(NULL, "\n");
    settings.clientId = strtok(NULL, "\n");
    
    file.close();

    return settings;
}