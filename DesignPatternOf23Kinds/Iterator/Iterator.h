/************************************************************************/
/* Iterator 模式也正是用来解决对一个聚合对象的遍历问题，将对聚合的遍历封装到一个
类中进行，这样就避免了暴露这个聚合对象的内部表示的可能。  Iterator模式的实现代码
很简单，实际上为了更好地保护 Aggregate 的状态，我们可以尽量减小Aggregate的public
接口， 而通过将Iterator对象声明位Aggregate的友元来给予Iterator一些特权，获得访问
Aggregate 私有数据和方法的机会。 */
/************************************************************************/
//Iterator.h
#ifndef _ITERATOR_H_
#define _ITERATOR_H_
class Aggregate;
typedef int Object;


class Iterator
{
public:
    virtual ~Iterator();
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone()  = 0;
    virtual Object CurrentItem() = 0;
protected:
    Iterator();
private:
};

class ConcreteIterator: public Iterator
{
public:
    ConcreteIterator(Aggregate *ag , int idx = 0);
    ~ConcreteIterator();
    void First();
    void Next();
    bool IsDone();
    Object CurrentItem();
protected:
private:
    Aggregate *_ag;
    int _idx;
};
#endif //~_ITERATOR_H_ 
