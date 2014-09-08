//strategy.h 
#ifndef _STRATEGY_H_ 
#define _STRATEGY_H_ 
#include <iostream>
using namespace std;

class Strategy 
{ 
public: 
	Strategy(){	} 
	virtual ~Strategy()
	{
		cout<<"~Strategy....."<<endl; 
	}
	virtual void AlgrithmInterface() = 0{	} 
}; 


class ConcreteStrategyA:public Strategy 
{ 
public: 
	ConcreteStrategyA(){	 }
	virtual ~ConcreteStrategyA()
	{
		cout<<"~ConcreteStrategyA....."<<endl;
	}
	void AlgrithmInterface()
	{
		cout<<"test ConcreteStrategyA....."<<endl; 
	}
}; 


class ConcreteStrategyB:public Strategy 
{ 
public: 
	ConcreteStrategyB(){	} 
	virtual ~ConcreteStrategyB()
	{
		cout<<"~ConcreteStrategyB....."<<endl;
	}
	void AlgrithmInterface()
	{
		cout<<"test ConcreteStrategyB....."<<endl; 
	}
}; 
#endif //~_STRATEGY_H_