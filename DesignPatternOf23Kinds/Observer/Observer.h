/************************************************************************/
/*  Observer 模式要解决的问题为：建立一个一（Subject）对多（Observer）的依赖关系，
并且做到当“一”变化的时候，依赖这个“一”的多也能够同步改变。最常见的一个例子就是：
对同一组数据进行统计分析时候，我们希望能够提供多种形式的表示 （例如以表格进行统计显
示、 柱状图统计显示、 百分比统计显示等） 。这些表示都依赖于同一组数据，我们当然需要
当数据改变的时候，所有的统计的显示都能够同时改变。Observer 模式就是解决了这一个问题。
Observer 的一个实例----- Model/View/Control（MVC）*/
/************************************************************************/
//Observer.h
#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include "Subject.h"
#include <string>
using namespace std;
typedef string State;

class Observer
{
public:
    virtual ~Observer();
    virtual void Update(Subject *sub) = 0;
    virtual void PrintInfo() = 0;
protected:
    Observer();
    State _st;
private:
};

class ConcreteObserverA: public Observer
{
public:
    virtual Subject *GetSubject();

    ConcreteObserverA(Subject *sub);
    virtual ~ConcreteObserverA();
    //传入 Subject 作为参数，这样可以让一个 View 属于多个的 Subject。
    void  Update(Subject *sub);
    void PrintInfo();
protected:
private:
    Subject *_sub;
};

class ConcreteObserverB: public Observer
{
public:
    virtual Subject *GetSubject();

    ConcreteObserverB(Subject *sub);
    virtual ~ConcreteObserverB();
    //传入 Subject 作为参数，这样可以让一个 View 属于多个的 Subject。
    void  Update(Subject *sub);
    void PrintInfo();
protected:
private:
    Subject *_sub;
};
#endif //~_OBSERVER_H_ 
