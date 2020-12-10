#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

class Command
{
    public:
        virtual String getCode();
        virtual String getOperationName();
        virtual int execute(char* payload);
};

typedef Command* CommandPtr;

class CommandHandler
{
    private:
        CommandPtr *_commands;
        int _size;
        int _maxCommands;
    public:
        CommandHandler(int maxCommands);
        ~CommandHandler();
        void registerCommand(Command *command);
        int handleCommand(const char* templateCode, char* payload);
        String getSupportedOperations();
};

class ShellCommand : public Command
{
    private:
        int _state;
    public:
        ShellCommand();
        String getCode();
        String getOperationName();
        int execute(char* payload);
};

class FirmwareUpdateCommand : public Command
{
    public:
        FirmwareUpdateCommand();
        String getCode();
        String getOperationName();
        int execute(char* payload);
};