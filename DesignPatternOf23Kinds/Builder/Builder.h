//Builder.h
#ifndef _BUILDER_H_
#define _BUILDER_H_

#include "Product.h"
#include <iostream>
#include <string>
using namespace std;

class Product;


class Builder
{
public:
    virtual ~Builder() {}
    virtual void BuildPartA(const string &buildPara) = 0;
    virtual void BuildPartB(const string &buildPara) = 0;
    virtual void BuildPartC(const string &buildPara) = 0;
    virtual Product *GetProduct() = 0;
protected:
    Builder() {}
private:
};


class ConcreteBuilder: public Builder
{
public:
    ConcreteBuilder() {}
    ~ConcreteBuilder() {}
    void BuildPartA(const string &buildPara)
    {
        cout << "Step1:Build PartA..." << buildPara << endl;
    }
    void BuildPartB(const string &buildPara)
    {
        cout << "Step1:Build PartB..." << buildPara << endl;
    }
    void BuildPartC(const string &buildPara)
    {
        cout << "Step1:Build PartC..." << buildPara << endl;
    }
    Product *GetProduct()
    {
        BuildPartA("pre-defined");
        BuildPartB("pre-defined");
        BuildPartC("pre-defined");
        return new Product();
    }

};
#endif //~_BUILDER_H_ 