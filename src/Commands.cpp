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

int CommandHandler::handleCommand(const char *commandCode, const char *payload)
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

BuiltinLedCommand::BuiltinLedCommand()
{
    pinMode(LED_BUILTIN, OUTPUT);
    this->_state = HIGH;
    digitalWrite(LED_BUILTIN, this->_state);
};

String BuiltinLedCommand::getCode()
{
    return "LED";
};

int BuiltinLedCommand::execute(const char *payload)
{
    Serial.printf("BuiltinLedCommand::payload: %s \n", payload);
    if (strcmp("ON", payload) == 0)
    {
        this->_state = LOW;
    }
    else
    {
        this->_state = HIGH;
    }

    digitalWrite(LED_BUILTIN, this->_state);
    return 0;
};