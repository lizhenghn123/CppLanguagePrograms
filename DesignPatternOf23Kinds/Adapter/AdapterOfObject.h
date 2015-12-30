/************************************************************************/
/* 类模式的 Adapter 采用继承的方式复用 Adaptee的接口，
   对象模式的 Adapter中采用组合的方式实现 Adaptee的复用 */
/************************************************************************/

#ifndef _ADAPTEROfObject _H_
#define _ADAPTEROfObject _H_
#include <iostream>


class TargetOfObject
{
public:
    TargetOfObject() {	}
    virtual ~TargetOfObject () {	}
    virtual void Request()
    {
        std::cout << "TargetOfObject::Request" << std::endl;
    }
};

class AdapteeOfObject
{
public:
    AdapteeOfObject () {	}
    ~AdapteeOfObject () {	}
    void SpecificRequest()
    {
        std::cout << "AdapteeOfObject::SpecificRequest" << std::endl;
    }
};

class AdapterOfObject : public TargetOfObject
{
public:
    AdapterOfObject (AdapteeOfObject *ade)
    {
        this->_ade = ade;
    }
    ~AdapterOfObject ()
    {
        delete _ade;
    }
    void Request()
    {
        _ade->SpecificRequest();
    }
private:
    AdapteeOfObject *_ade;
};

#endif //~_ADAPTEROfObject _H_ 

