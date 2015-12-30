//Reciever.h
#ifndef _RECIEVER_H_
#define _RECIEVER_H_
#include <iostream>

class RecieverA
{
public:
    RecieverA() {	}
    ~RecieverA() {	}
    void Action()
    {
        std::cout << "RecieverA action......." << std::endl;
    }
protected:
private:
};

class RecieverB
{
public:
    RecieverB() {	}
    ~RecieverB() {	}
    void Action()
    {
        std::cout << "RecieverB action......." << std::endl;
    }
protected:
private:
};


class SimpleReciever
{
public:
    SimpleReciever() {	}
    ~SimpleReciever() {	}
    void Action()
    {
        std::cout << "SimpleReciever action......." << std::endl;
    }
protected:
private:
};


#endif //~_RECIEVER_H_