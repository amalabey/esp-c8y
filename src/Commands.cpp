#ifndef COMMANDS_H
#define COMMANDS_H
#include "Commands.h"
#endif

#include <string.h>

#define LED_BUILTIN 10
const int MAX_COMMANDS = 10;

CommandHandler::CommandHandler(int maxCommands)
{
    this->_maxCommands = maxCommands;
    this->_commands = new CommandPtr[maxCommands];
    this->_size = 0;
};

CommandHandler::~CommandHandler()
{
    if (this->_commands != NULL)
        free(this->_commands);
};

void CommandHandler::registerCommand(Command *command)
{
    if (this->_size >= this->_maxCommands)
    {
        throw "Max commands reached";
    }

    this->_commands[this->_size++] = command;
};

int CommandHandler::handleCommand(const char *commandCode, char *payload)
{
    Serial.printf("command: %s, payload: %s \n", commandCode, payload);
    if (this->_commands == NULL)
        return -1;

    for (int i = 0; i < this->_size; i++)
    {
        CommandPtr command = this->_commands[i];
        if (command->getCode() == commandCode)
        {
            command->execute(payload);
            return 0;
        }
    }

    return -1;
};

String CommandHandler::getSupportedOperations()
{
    String operations = "";

    for (int i = 0; i < this->_size; i++)
    {
        CommandPtr command = this->_commands[i];
        if (command != NULL)
        {
            if(operations.length() > 0) operations += ",";
            operations += command->getOperationName();
        }
    }

    return operations;
}

ShellCommand::ShellCommand()
{
    pinMode(LED_BUILTIN, OUTPUT);
    this->_state = HIGH;
    digitalWrite(LED_BUILTIN, this->_state);
};

String ShellCommand::getCode()
{
    return "511";
};

String ShellCommand::getOperationName()
{
    return "c8y_Command";
};

int ShellCommand::execute(char *payload)
{
    Serial.printf("ShellCommand::payload: %s\n", payload);
    char *commandCode = strtok(payload, " ");
    char *commandPayload = strtok(NULL, " ");
    Serial.printf("commandCode:%s, commandPaylod:%s\n", commandCode, commandPayload);
    if (strcmp(commandCode, "LED") == 0)
    {
        if (strcmp("ON", commandPayload) == 0)
        {
            this->_state = LOW;
        }
        else
        {
            this->_state = HIGH;
        }
    }
    Serial.printf("LED state: %d", this->_state);
    digitalWrite(LED_BUILTIN, this->_state);
    return 0;
};

FirmwareUpdateCommand::FirmwareUpdateCommand()
{
};

String FirmwareUpdateCommand::getCode()
{
    return "515";
};

String FirmwareUpdateCommand::getOperationName()
{
    return "c8y_Firmware";
};

int FirmwareUpdateCommand::execute(char *payload)
{
    Serial.printf("FirmwareUpdateCommand::payload: %s \n", payload);
    
    return 0;
};