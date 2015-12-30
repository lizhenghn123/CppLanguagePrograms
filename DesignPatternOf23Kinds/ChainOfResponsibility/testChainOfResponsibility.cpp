#include "Handle.h"
#include <iostream>
#include <list>
using namespace std;

int main(int argc, char *argv[])
{
    Handle *h1 = new ConcreteHandleA();
    Handle *h2 = new ConcreteHandleB();
    h1->SetSuccessor(h2);
    h1->HandleRequest();

    cout << "//--------------------------------------------------\n"; //lizheng 2011-11-15 15:32:30
    //下面是我添加的，将能对某请求进行处理的类放在一个链表中，当该请求发生时，找到一个能处理该请求的类对象
    //比如有多个类能响应鼠标事件，但一般当鼠标事件发生时只需一个类对象来处理即可。
    list<Handle *> listHandle;
    listHandle.push_back(h1);
    listHandle.push_back(h2);
    listHandle.push_back(new DefaultHandle);
    //发生鼠标事件，找到某个对象处理该事件
    for(list<Handle *>::const_iterator  p = listHandle.begin(); p != listHandle.end(); p++)
    {
        int rt = (*p)->HandleMouseClick();
        if(rt == 1) //找到该对象，该次鼠标事件已被处理
        {
            break;
        }
    }

    system("pause");
    return 0;
}