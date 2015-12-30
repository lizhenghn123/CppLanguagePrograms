//Invoker.h
#ifndef _INVOKER_H_
#define _INVOKER_H_
#include "Invoker.h"
#include "Command.h"
#include <iostream>

class Command;


class Invoker
{
public:
    Invoker(Command *cmd)
    {
        this->_cmd = cmd;
    }
    ~Invoker()
    {
        delete _cmd;
    }
    void Invoke()
    {
        _cmd->Execute();
    }
protected:
private:
    Command *_cmd;
};
#endif //~_INVOKER_H_ 
