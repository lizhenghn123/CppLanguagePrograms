//Factory.h 
#ifndef _FACTORY_H_ 
#define _FACTORY_H_ 

#include "Factory.h" 
#include "Product.h" 
#include <iostream> 
using namespace std; 


class Product; 

class Factory 
{ 
public: 
	virtual ~Factory() = 0{	} ;
	virtual Product* CreateProduct() = 0; 
protected: 
	Factory(){	}; 
}; 


class ConcreteFactory:public Factory 
{ 
public: 	 
	ConcreteFactory()
	{
		cout<<"ConcreteFactory....."<<endl; 
	}
	~ConcreteFactory(){	}
	Product* CreateProduct()
	{
		return new ConcreteProduct(); 
	}
}; 
#endif //_FACTORY_H_ 


