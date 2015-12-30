/************************************************************************/
/* 在开发中，我们经常可能要递归构建树状的组合结构，Composite 模式则提供了很好的
解决方案。Composite 模式在实现中有一个问题就是要提供对于子节点（Leaf）的管理策略，
这里使用的是 STL  中的vector，可以提供其他的实现方式，如数组、链表、Hash 表等。*/
/************************************************************************/

//Component.h
#ifndef _COMPONENT_H_
#define _COMPONENT_H_
class Component
{
public:
    Component() {	}
    virtual ~Component() {	}
public:
    virtual void Operation() = 0;
    virtual void Add(const Component & ) {	}
    virtual void Remove(const Component & ) {	}
    virtual Component *GetChild(int )
    {
        return 0;
    }
protected:
private:
};
#endif //~_COMPONENT_H_ 