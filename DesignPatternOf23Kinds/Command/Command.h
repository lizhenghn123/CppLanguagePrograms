/************************************************************************/
/*  Command 模式通过将请求封装到一个对象（Command）中，并将请求的接受者存放到
具体的 ConcreteCommand 类中（Receiver）中，从而实现调用操作的对象和操作的具体
实现者之间的解耦。 Command 模式结构图中，将请求的接收者（处理者）放到 Command
的具体子类ConcreteCommand 中，当请求到来时（Invoker 发出 Invoke 消息激活 
Command 对象）,ConcreteCommand 将处理请求交给 Receiver对象进行处理。
	Command 模式在实现的实现和思想都很简单，其关键就是将一个请求封装到一个类中
（Command） ，再提供处理对象（Receiver） ，最后Command 命令由 Invoker激活。
	支持取消操作。Command的Excute操作可在实施操作前将状态存储起来，在取消操作时
这个状态用来消除该操作的影响。Command接口必须添加一个UnExcute操作，该操作取消上
一次Excute调用的效果。执行的命令被存储在一个历史列表中。可通过向后和向前遍历这
一列表并分别调用UnExcute和Excute来实现重数不限的“取消”和“重做” 。*/
/************************************************************************/
//Command.h 
#ifndef _COMMAND_H_ 
#define _COMMAND_H_ 
#include <iostream>
#include <list>
#include <vector>
#include "Reciever.h"
class Reciever; 

class Command 
{ 
public: 
	virtual ~Command(){	}
	virtual void Execute() = 0; 
protected: 
	Command(){	}
private: 
}; 


class ConcreteCommandA : public Command 
{ 
public: 
	ConcreteCommandA(RecieverA* rev)
	{
		this->_rev = rev;
	}
	~ConcreteCommandA()
	{
		delete _rev;
	} 
	void Execute()
	{
		this->_rev->Action(); 
		std::cout<<"ConcreteCommandA..."<<std::endl; 
	}
protected: 
private: 
	RecieverA* _rev; 
}; 
class ConcreteCommandB : public Command 
{ 
public: 
	ConcreteCommandB(RecieverB* rev)
	{
		this->_rev = rev;
	}
	~ConcreteCommandB()
	{
		delete _rev;
	} 
	void Execute()
	{
		this->_rev->Action(); 
		std::cout<<"ConcreteCommandB..."<<std::endl; 
	}
protected: 
private: 
	RecieverB* _rev; 
}; 
//----------------------------------------------------------------------------------------------
/************************************************************************/
/* 我们可以将请求接收者的处理抽象出来作为参数传给 Command 对象，实际也就是
回调的机制（Callback）来实现这一点，也就是说将处理操作方法地址（在对象内部）
通过参数传递给Command 对象，Command 对象在适当的时候（Invoke 激活的时候）
再调用该函数。。这里就要用到 C++中的类成员函数指针的概念           
对于简单的不能取消和不需参数的命令 , 可以用一个类模板来参数化该命令的接收者。我
们将为这些命令定义一个模板子类 SimpleCommand. 用SimpleReciever类型参数化SimpleCommand，
并维护一个接收者对象和一个动作之间的绑定 ,而这一动作是用指向一个成员函数的指针存储
的。
	这一方案仅适用于简单命令。更复杂的命令不仅要维护它们的接收者，而且还要登
记参数，有时还要保存用于取消操作的状态。*/
/************************************************************************/
template <class Reciever> 
class SimpleCommand : public Command 
{ 
public: 
	typedef void (Reciever::* Action)(); 
	SimpleCommand(Reciever* rev,Action act) //构造器存储接收者和对应实例变量中的动作。Excute操作实施接收者的这个动作。
	{ 
		_rev = rev; 
		_act = act; 
	} 
	virtual void Execute() 
	{ 
		(_rev->* _act)(); 
	} 
	~SimpleCommand() 
	{ 
		delete _rev; 
	} 
protected: 
private: 
	Reciever* _rev; 
	Action _act; 
}; 

//----------------------------------------------------------------------------------------------
/************************************************************************/
/*MacroCommand管理一个子命令序列，它提供了增加和删除子命令的操作。这里不需要显式的
接收者，因为这些子命令已经定义了它们各自的接收者。MacroCommand的关键是它的Exectue成
员函数。它遍历所有的子命令并调用其各自的Exectue操作。
注意，如果MacroCommand实现取消操作, 那么它的子命令必须以相对于Exectue的实现相反的顺
序执行各子命令的取消操作。最后, MacroCommand必须提供管理它的子命令的操作。
MacroCommand也负责删除它的子命令。*/
/************************************************************************/

class MacroCommand : public Command 
{
public:	
	MacroCommand(){	}
	virtual ~MacroCommand(){	}

	virtual void Add(Command* com)
	{
		this->_cmds.push_front(com);
	}
	virtual void Remove(Command* com)
	{
		_cmds.remove(com);
	}
	virtual void Execute()
	{
		for(std::list<Command*> ::iterator p  = _cmds.begin();p!=_cmds.end();p++)
		{
			Command* c = *p;
			c->Execute();
		}
	}
private:
	std::list< Command* >  _cmds;
};



#endif //~_COMMAND_H_ 
