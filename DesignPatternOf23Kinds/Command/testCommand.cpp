#include "Command.h"
#include "Invoker.h"
#include "Reciever.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    RecieverA *rev = new RecieverA();
    Command *cmd = new ConcreteCommandA(rev);
    Invoker *inv = new Invoker(cmd);
    inv->Invoke();
    cout << "----------------------------------------------------\n";

    //用来演示使用 C++中的类成员函数指针 的方法
    SimpleReciever *rev2 = new SimpleReciever();
    Command *cmd2 = new SimpleCommand<SimpleReciever>(rev2, &SimpleReciever::Action);
    cmd2->Execute();
    cout << "----------------------------------------------------\n";

    //下面演示一个管理子命令序列
    MacroCommand *mcom = new MacroCommand();
    mcom->Add(new ConcreteCommandA(new RecieverA));
    mcom->Add(new ConcreteCommandB(new RecieverB));
    mcom->Execute();

    system("pause");
    return 0;
}
