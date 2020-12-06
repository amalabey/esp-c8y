#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "Configuration.h"
#endif
#include <string.h>

#include "Credentials.h"

const char *CONFIG_FILE_NAME = "/settings.txt";
const char END_MARKER = '\n';

int readLine(HardwareSerial &serial, const char* prompt, char* line, int maxSize)
{
    serial.println(prompt);
    char rc;
    int index = 0;

    while (true)
    {
        if(serial.available() > 0)
        {
            rc = serial.read();
            serial.print(rc);
            if(rc != END_MARKER && index < maxSize)
            {
                line[index] = rc;        
                index++;
            }else
            {
                line[index] = '\0';
                return index;
            }
        }
    }
}

Settings Configuration:: readFromSerial(HardwareSerial &serial)
{
    char *buffer = new char[1024];
    readLine(serial, "Enter connection parameters (wifi-ssid,wifi-psk,host,tenant,user,pwd,clientid):", buffer, 1024);

    Settings settings;
    settings.wifiSsid = strtok(buffer, ",");
    settings.wifiPassword = strtok(NULL, ",");
    settings.hostName = strtok(NULL, ",");
    settings.tenantId = strtok(NULL, ",");
    settings.userName = strtok(NULL, ",");
    settings.password = strtok(NULL, ",");
    settings.clientId = strtok(NULL, ",");

    return settings;
}

Settings Configuration::getSettings(fs::FS &fs, HardwareSerial &serial)
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

void Configuration::persistSettings(fs::FS &fs, Settings settings)
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


Settings Configuration::readFromFileSystem(fs::FS &fs)
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

    Settings settings;
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