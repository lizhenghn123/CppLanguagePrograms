#ifndef _DECORATOR_H_ 
#define _DECORATOR_H_ 
#include <iostream> 


class Component 
{ 
public: 
	virtual ~Component(){	} 
	virtual void  Operation(){	} 
protected: 
	Component(){	} 
private: 
}; 

class ConcreteComponent:public Component 
{ 
public: 
	ConcreteComponent(){	} 
	~ConcreteComponent(){	} 
	void  Operation()
	{
		std::cout<<"ConcreteComponent operation..."<<std::endl; 
	}
}; 
class Decorator:public Component 
{ 
public: 
	Decorator(Component* com)
	{
		this->_com = com; 
	}
	virtual ~Decorator()
	{
		delete _com;
	}
	void  Operation(){	} 
protected: 
	Component* _com; 
private: 
}; 


class ConcreteDecorator:public Decorator 
{ 
public: 
	ConcreteDecorator(Component* com):Decorator(com) 
	{

	}
	~ConcreteDecorator(){	} 
	void  Operation()
	{
		_com->Operation(); 
		this->AddedBehavior(); 
	}
	void AddedBehavior()
	{
		std::cout<<"ConcreteDecorator::AddedBehacior...."<<std::endl; 
	} 

}; 


#endif //~_DECORATOR_H_ 
 
  