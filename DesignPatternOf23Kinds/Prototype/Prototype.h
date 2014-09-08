//Prototype.h 
#ifndef _PROTOTYPE_H_ 
#define _PROTOTYPE_H_ 

#include <iostream> 
using namespace std; 


class Prototype 
{ 
public: 
	virtual ~Prototype(){} 
	virtual Prototype* Clone() const = 0; 
protected: 
	Prototype(){}
private: 
}; 

class ConcretePrototype:public Prototype 
{ 
public: 
	ConcretePrototype(){}
	ConcretePrototype(const ConcretePrototype& cp)
	{
		cout<<"ConcretePrototype copy ..."<<endl; 
	}
	~ConcretePrototype(){}
	Prototype* Clone() const
	{
		return new ConcretePrototype(*this); 
	}
protected: 
private: 
}; 


Prototype* Prototype::Clone() const 
{ 
	return 0; 
} 


#endif //~_PROTOTYPE_H_ 