#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Configuration.h"
#endif
#include <string.h>

#include "Credentials.h"

const char *CONFIG_FILE_NAME = "/settings.txt";

Settings_t Configuration:: readFromSerial(HardwareSerial &serial)
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

Settings_t Configuration::getSettings(fs::FS &fs, HardwareSerial &serial)
{
    if(this->isPersisted(fs))
    {
        return this->readFromFileSystem(fs);
    }else{
        return this->readFromSerial(serial);
    }
}

bool Configuration::isPersisted(fs::FS &fs)
{
    return fs.exists(CONFIG_FILE_NAME);
}

void Configuration::persistSettings(fs::FS &fs, Settings_t settings)
{
    Serial.printf("Writing file: %s\r\n", CONFIG_FILE_NAME);

    File file = fs.open(CONFIG_FILE_NAME, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    file.println(settings.wifiSsid);
    file.println(settings.wifiPassword);
    file.println(settings.hostName);
    file.println(settings.tenantId);
    file.println(settings.userName);
    file.println(settings.password);
    file.println(settings.clientId);

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
    Serial.printf("Buffer: %s \r\n", buffer);

    Settings_t settings;
    settings.wifiSsid = strtok(buffer, "\r\n");
    settings.wifiPassword = strtok(NULL, "\r\n");
    settings.hostName = strtok(NULL, "\r\n");
    settings.tenantId = strtok(NULL, "\r\n");
    settings.userName = strtok(NULL, "\r\n");
    settings.password = strtok(NULL, "\r\n");
    settings.clientId = strtok(NULL, "\r\n");
    
    file.close();

    return settings;
}