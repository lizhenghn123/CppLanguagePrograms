//Visitor.h
#ifndef _VISITOR_H_
#define _VISITOR_H_
#include "Element.h"
#include <iostream>
using namespace std;


class Element;

class Visitor
{
public:
    virtual ~Visitor() { }
    virtual void VisitConcreteElementA(Element *elm) = 0;
    virtual void VisitConcreteElementB(Element *elm) = 0;
protected:
    Visitor() { }
private:
};


class ConcreteVisitorA: public Visitor
{
public:
    ConcreteVisitorA() {	}
    virtual ~ConcreteVisitorA() {	}
    virtual void VisitConcreteElementA(Element *elm)
    {
        cout << "ConcreteVisitorA will visit ConcreteElementA..." << endl;
    }
    virtual void VisitConcreteElementB(Element *elm)
    {
        cout << "ConcreteVisitorA will visit ConcreteElementB..." << endl;
    }
};


class ConcreteVisitorB: public Visitor
{
public:
    ConcreteVisitorB() {	}
    virtual ~ConcreteVisitorB() {	}
    virtual void VisitConcreteElementA(Element *elm)
    {
        cout << "ConcreteVisitorB will visit ConcreteElementA..." << endl;
    }
    virtual void VisitConcreteElementB(Element *elm)
    {
        cout << "ConcreteVisitorB will visit ConcreteElementB..." << endl;
    }
};


#endif //~_VISITOR_H_ 
