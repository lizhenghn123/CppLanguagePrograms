/************************************************************************/
/* Chain of  Responsibility 模式描述其实就是这样一类问题将可能处理一个请求的对象链
接成一个链，并将请求在这个链上传递，直到有对象处理该请求（可能需要提供一个默认处理
所有请求的类，例如 MFC 中的 CwinApp 类） 。 Chain of Responsibility模式中 
ConcreteHandler将自己的后继对象（向下传递消息的对象）记录在自己的后继表中，当一个请
求到来时，ConcreteHandler 会先检查看自己有没有匹配的处理程序， 如果有就自己处理， 
否则传递给它的后继。 当然这里示例程序中为了简化，ConcreteHandler 只是简单的检查看自
己有没有后继，有的话将请求传递给后继进行处理，没有的话就自己处理。 

Chain of Responsibility模式的最大的一个优点就是给系统降低了耦合性，请求的发送者完全
不必知道该请求会被哪个应答对象处理，极大地降低了系统的耦合性。                                                                      */
/************************************************************************/
//Handle.h 
#ifndef _HANDLE_H_ 
#define _HANDLE_H_ 
#include <iostream> 
using namespace std; 


class Handle 
{ 
public: 
	virtual ~Handle()
	{
		delete _succ;
	}
	virtual void HandleRequest()=0; 
	virtual int HandleMouseClick()=0;
	void SetSuccessor(Handle* succ)
	{
		this->_succ = succ;
	}
	Handle* GetSuccessor()
	{
		return _succ;
	}
protected: 
	Handle()
	{
		_succ = 0;
	}
	Handle(Handle* succ)
	{
		this->_succ = succ;
	}
private: 
	Handle* _succ; 
}; 

class ConcreteHandleA:public Handle 
{ 
public: 
	ConcreteHandleA(){	}
	~ConcreteHandleA(){	}
	ConcreteHandleA(Handle* succ):Handle(succ){	}
	void HandleRequest()
	{
		if (this->GetSuccessor() != 0) 
		{ 
			cout<<"ConcreteHandleA 我把处理权给后继节点....."<<endl; 
			this->GetSuccessor()->HandleRequest(); 
		} 
		else 
		{ 
			cout<<"ConcreteHandleA 没有后继了，我必须自己处理...."<<endl; 
		} 
	}

	int HandleMouseClick()
	{
		//做或不做事情，若该类处理该事件，返回1，否则返回0
		return 0;
	}
 
};


class ConcreteHandleB:public Handle 
{ 
public: 
	ConcreteHandleB(){	}
	~ConcreteHandleB(){	} 
	ConcreteHandleB(Handle* succ):Handle(succ){	}
	void HandleRequest()
	{
		if (this->GetSuccessor() != 0) 
		{ 
			cout<<"ConcreteHandleB 我把处理权给后继节点....."<<endl; 
			this->GetSuccessor()->HandleRequest(); 
		} 
		else 
		{ 
			cout<<"ConcreteHandleB 没有后继了，我必须自己处理...."<<endl; 
		} 
	}

	int HandleMouseClick()
	{
		//做或不做事情，若该类处理该事件，返回1，否则返回0
		cout<<"ConcreteHandleB处理了HandleMouseClick（）事件\n";
		return 1;
	}
}; 
//----------------------------------------lizheng 2011-11-15 15:32:18
//定义一个默认类，可默认处理所有请求
class DefaultHandle :public Handle
{
public: 
	DefaultHandle(){	}
	~DefaultHandle(){	} 
	DefaultHandle(Handle* succ):Handle(succ){	}
	void HandleRequest()
	{
		cout<<"无对象处理该事件，由默认类处理\n";
	}

	int HandleMouseClick()
	{
		//做或不做事情，若该类处理该事件，返回1，否则返回0
		cout<<"DefaultHandle处理了HandleMouseClick（）事件\n";
		return 1;
	}
};
#endif //~_HANDLE_H_ 