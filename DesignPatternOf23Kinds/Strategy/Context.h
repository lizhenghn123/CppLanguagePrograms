//Context.h 
#ifndef _CONTEXT_H_ 
#define _CONTEXT_H_ 

#include "Strategy.h"
class Strategy; 
/** 
*这个类是 Strategy模式的关键，也是 Strategy模式和 Template 模式的根本
区别所在。 
*Strategy 通过“组合”（委托）方式实现算法（实现）的异构，而 Template 模
式则采取的是继承的方式 
*这两个模式的区别也是继承和组合两种实现接口重用的方式的区别 
*/ 
class Context 
{ 
public: 
	Context(Strategy* stg)
	{
		_stg = stg; 
	}
	~Context()
	{
		if (!_stg) 
			delete _stg;
	}
	void DoAction()
	{
		 _stg->AlgrithmInterface(); 
	}
private: 
	Strategy* _stg; 
}; 
#endif //~_CONTEXT_H_ 
