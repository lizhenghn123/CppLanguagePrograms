//Composite.h
#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_
#include "Component.h"
#include <vector>
using namespace std;

class Composite: public Component
{
public:
    Composite() { }
    ~Composite() { }
public:
    void Operation()
    {
        vector<Component *>::iterator comIter = comVec.begin();
        for (; comIter != comVec.end(); comIter++)
        {
            (*comIter)->Operation();
        }
    }
    void Add(Component *com)
    {
        comVec.push_back(com);
    }
    void Remove(Component *com)
    {
        //comVec.erase(&com);
        for (vector<Component *>::iterator p = comVec.begin(); p != comVec.end(); p++)
        {
            if(*p == com)
                comVec.erase(p);
        }
    }
    Component *GetChild(int index)
    {
        return comVec[index];
    }
protected:
private:
    vector<Component *> comVec;
};
#endif //~_COMPOSITE_H_ 
