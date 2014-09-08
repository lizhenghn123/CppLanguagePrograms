//AbstractionImp.h 
#ifndef _ABSTRACTIONIMP_H_ 
#define _ABSTRACTIONIMP_H_ 

#include <iostream> 
using namespace std;


class AbstractionImp 
{ 
public: 
	virtual ~AbstractionImp(){} 
	virtual void Operation() = 0
	{
		cout<<"AbstractionImp....imp..."<<endl; 
	}
protected: 
	AbstractionImp(){}
private: 
}; 

class ConcreteAbstractionImpA:public AbstractionImp 
{ 
public: 
	ConcreteAbstractionImpA(){}
	~ConcreteAbstractionImpA(){} 
	virtual void Operation()
	{
		 cout<<"ConcreteAbstractionImpA...."<<endl; 
	}
protected: 

private: 
}; 

class ConcreteAbstractionImpB:public AbstractionImp 
{ 
public: 
	ConcreteAbstractionImpB(){}
	~ConcreteAbstractionImpB(){} 
	virtual void Operation()
	{
		cout<<"ConcreteAbstractionImpB...."<<endl; 
	}
protected: 

private: 
}; 


#endif //~_ABSTRACTIONIMP_H_ 
