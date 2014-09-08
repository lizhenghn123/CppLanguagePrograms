//FlyweightFactory.h 
#ifndef _FLYWEIGHTFACTORY_H_ 
#define _FLYWEIGHTFACTORY_H_ 
#include "Flyweight.h" 
#include <string> 
#include <vector>
#include <iostream> 
#include <cassert> 
using namespace std; 

class FlyweightFactory 
{ 
public: 
	FlyweightFactory(){	} 
	~FlyweightFactory(){	} 
	Flyweight* GetFlyweight(const string& key)
	{ 
		vector<Flyweight*>::iterator it = _fly.begin(); 
		for (; it != _fly.end();it++) 
		{ 
			//找到了，就一起用，^_^ 
			if ((*it)->GetIntrinsicState() == key) 
			{ 
				cout<<"already created by users...."<<key<<endl; 
				return *it; 
			} 
		} 
		Flyweight* fn = new ConcreteFlyweight(key); 
		_fly.push_back(fn); 
		return fn; 
	} 
private: 
	vector<Flyweight*> _fly; 
}; 


#endif //~_FLYWEIGHTFACTORY_H_ 
 