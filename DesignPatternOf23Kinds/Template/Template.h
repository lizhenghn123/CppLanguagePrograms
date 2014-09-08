//Template.h 

#ifndef _TEMPLATE_H_ 
#define _TEMPLATE_H_ 
#include <iostream> 
using namespace std;

class AbstractClass 
{ 
public: 
	virtual ~AbstractClass(){	} 
	void TemplateMethod()
	{
		this->PrimitiveOperation1(); 
		this->PrimitiveOperation2(); 

	}
protected: 
	virtual void PrimitiveOperation1() = 0; 
	virtual void PrimitiveOperation2() = 0; 
	AbstractClass(){	} 
}; 

class ConcreteClass1:public AbstractClass 
{ 
public: 
	ConcreteClass1(){	} 
	~ConcreteClass1(){	}

protected: 
	void PrimitiveOperation1()
	{
		cout<<"ConcreteClass1...PrimitiveOperat ion1"<<endl; 
	}
	void PrimitiveOperation2()
	{
		cout<<"ConcreteClass1...PrimitiveOperat ion2"<<endl; 
	}
}; 

class ConcreteClass2:public AbstractClass 
{ 
public: 
	ConcreteClass2(){	} 
	~ConcreteClass2(){	}

protected: 
	void PrimitiveOperation1()
	{
		cout<<"ConcreteClass2...PrimitiveOperat ion1"<<endl; 
	}
	void PrimitiveOperation2()
	{
		cout<<"ConcreteClass2...PrimitiveOperat ion2"<<endl; 
	}
}; 

#endif //~_TEMPLATE_H_ 
