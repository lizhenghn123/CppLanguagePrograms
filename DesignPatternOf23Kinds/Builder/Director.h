//Director.h
#ifndef _DIRECTOR_H_
#define _DIRECTOR_H_

#include "Builder.h"
class Builder;


class Director
{
public:
    Director(Builder *bld)
    {
        _bld = bld ;
    }
    ~Director() {}
    void Construct()
    {
        _bld->BuildPartA("user-defined");
        _bld->BuildPartB("user-defined");
        _bld->BuildPartC("user-defined");
    }
protected:
private:
    Builder *_bld;
};


#endif //~_DIRECTOR_H_ 

