//Flyweight.h
#ifndef _FLYWEIGHT_H_
#define _FLYWEIGHT_H_
#include <string>
#include <iostream>
using namespace std;

class Flyweight
{
public:
    virtual ~Flyweight() {	}
    virtual void Operation(const string &extrinsicState)
    {

    }
    string GetIntrinsicState()
    {
        return this->_intrinsicState;
    }
protected:
    Flyweight(string intrinsicState)
    {
        this->_intrinsicState = intrinsicState;
    }
private:
    string _intrinsicState;
};


class ConcreteFlyweight: public Flyweight
{
public:
    ConcreteFlyweight(string intrinsicState): Flyweight(intrinsicState)
    {
        cout << "ConcreteFlyweight Build....." << intrinsicState << endl;
    }
    ~ConcreteFlyweight() {	}
    void Operation(const string &extrinsicState)
    {
        cout << "ConcreteFlyweight:ÄÚÔÌ[" << this->GetIntrinsicState() << "] ÍâÔÌ[" << extrinsicState << "]" << endl;
    }
};

#endif //~_FLYWEIGHT_H_ 