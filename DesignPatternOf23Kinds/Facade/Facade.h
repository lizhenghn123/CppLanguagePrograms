/************************************************************************/
/*Façade 模式在高层提供了一个统一的接口，解耦了系统。设计模式中还有另一种模式
Mediator 也和 Façade 有类似的地方。但是 Mediator 主要目的是对象间的访问的解耦*/
/************************************************************************/
//Facade.h 
#ifndef _FACADE_H_ 
#define _FACADE_H_ 
#include <iostream> 
using namespace std; 


class Subsystem1 
{ 
public: 
	Subsystem1(){	} 
	~Subsystem1(){	}
	void Operation()
	{
		cout<<"Subsystem1 operation.."<<endl;
	}
}; 

class Subsystem2 
{ 
public: 
	Subsystem2(){	} 
	~Subsystem2(){	} 
	void Operation()
	{
		cout<<"Subsystem2 operation.."<<endl;
	}
}; 

class Facade 
{ 
public: 
	Facade()
	{
		this->_subs1 = new Subsystem1(); 
		this->_subs2 = new Subsystem2();
	}
	~Facade()
	{
		delete _subs1; 
		delete _subs2; 
	}
	void OperationWrapper()
	{
		this->_subs1->Operation(); 

		this->_subs2->Operation(); 
	}
private: 
	Subsystem1* _subs1; 
	Subsystem2* _subs2; 
}; 

#endif //~_FACADE_H_ 