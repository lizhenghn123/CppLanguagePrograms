//Element.h
#ifndef _ELEMENT_H_
#define _ELEMENT_H_

class Visitor;

class Element
{
public:
    virtual ~Element();
    virtual void Accept(Visitor *vis) = 0;
protected:
    Element();
};

class ConcreteElementA: public Element
{
public:
    ConcreteElementA();
    ~ConcreteElementA();
    void Accept(Visitor *vis);
};

class ConcreteElementB: public Element
{
public:
    ConcreteElementB();
    ~ConcreteElementB();
    void Accept(Visitor *vis);
};

#endif //~_ELEMENT_H_ 