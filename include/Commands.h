#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

class Command
{
    public:
        virtual String getCode();
        virtual int execute(const char* payload);
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
        int handleCommand(const char* templateCode, const char* payload);
};

class BuiltinLedCommand : public Command
{
    private:
        int _state;
    public:
        BuiltinLedCommand();
        String getCode();
        int execute(const char* payload);
};