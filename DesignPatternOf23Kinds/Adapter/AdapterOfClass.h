/************************************************************************/
/* 类模式的 Adapter 采用继承的方式复用 Adaptee的接口，
   对象模式的 Adapter中采用组合的方式实现 Adaptee的复用 */
/************************************************************************/

#ifndef _ADAPTEROfClass_H_ 
#define _ADAPTEROfClass_H_

#include <iostream> 

class TargetOfClass
{ 
public: 
	TargetOfClass(){	} 
	virtual ~TargetOfClass(){	} 
	virtual void Request()
	{
		std::cout<<"TargetOfClass::Request"<<std::endl;
	}
}; 


class AdapteeOfClass 
{ 
public: 
	AdapteeOfClass(){	} 
	~AdapteeOfClass(){	}
	void SpecificRequest()
	{
		std::cout<<"AdapteeOfClass::SpecificRequest"<<std::endl; 
	}
}; 

class AdapterOfClass:public TargetOfClass,private AdapteeOfClass 
{ 
public: 
	AdapterOfClass(){	} 
	~AdapterOfClass(){	}
	void Request()
	{
		this->SpecificRequest();
	}
}; 


#endif //~_ADAPTEROfClass_H_ 
