//Leaf.h
#ifndef _LEAF_H_
#define _LEAF_H_
#include "Component.h"
#include <iostream>
using namespace std;


class Leaf: public Component
{
public:
    Leaf() {	}
    ~Leaf() {	}
    void Operation()
    {
        cout << "Leaf operation....." << endl;
    }
};
#endif //~_LEAF_H_ 