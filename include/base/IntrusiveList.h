#pragma  once

// 目前只能实现自定义类型的侵入式实现，对非内置基本类型（int等）还无法做到
template <class ListableClass>
class Listable
{
public:
    Listable() : next_(NULL), prev_(NULL)
    {
    }

    /** 得到下一个可链表对象 */
    ListableClass* next() const { return next_; }

    /** 得到前一个可链表对象 */
    ListableClass* prev() const { return prev_; }

    /** 关联下一个可链表对象 */
    void setNext(ListableClass *next) { next_ = next; }

    /** 关联前一个可链表对象 */
    void setPrev(ListableClass *prev) { prev_ = prev; }

private:
    ListableClass* next_;
    ListableClass* prev_;
};

template<>
class Listable < int >   // not ok
{
public:
    Listable(int *curr = 0) : next_(NULL), prev_(NULL)
    {
    }

    /** 得到下一个可链表对象 */
    Listable < int > * next() const { return next_; }

    /** 得到前一个可链表对象 */
    Listable < int > * prev() const { return prev_; }

    /** 关联下一个可链表对象 */
    void setNext(Listable < int >  *next) { next_ = next; }

    /** 关联前一个可链表对象 */
    void setPrev(Listable < int >  *prev) { prev_ = prev; }

private:
    Listable < int >   *next_;
    Listable < int >   *prev_;
};

template <class ListableClass>
class ListQueue
{
public:
    ListQueue()
        : size_(0)
        , head_(NULL)
        , tail_(NULL)
    {
        head_ = new ListableClass;
        tail_ = head_;
    }

    ~ListQueue()
    {
        delete head_;
        tail_ = NULL;
    }

    /** 得到队列中元素个数 */
    int size() const
    {
        return size_;
    }

    /** 判断队列是否为空 */
    bool empty() const
    {
        return 0 == size_;
    }

    /** 得到指向队首对象的指针 */
    ListableClass* front() const
    {
        return (ListableClass*)head_->next();
    }

    /** 在队尾添加一个可链表对象 */
    void push(ListableClass* listable)
    {
        assert(listable != NULL);
        if (NULL == listable) return;

        ListableClass* next = listable->next();
        ListableClass* prev = listable->prev();
        if (prev != NULL) return; // 已经在队列中
        assert(NULL == next);

        listable->setNext(NULL);
        listable->setPrev(tail_);
        tail_->setNext(listable);
        tail_ = listable;
        ++size_;
    }

	/**
	* 将一个可链表对象从队列中删除
	* 删除操作是高效的，因为0查找，只需要解除链接关系即可
	*/
    void remove(ListableClass* listable)
    {
        assert(listable != NULL);
        if (NULL == listable) return;

        ListableClass* next = listable->next();
        ListableClass* prev = listable->prev();
        if (NULL == prev)
        {
            assert(NULL == next);
            return; // 已经不在队列中
        }

        prev->setNext(next);
        if (NULL == next)
        {
            // 尾结点的next才会为NULL
            tail_ = prev;
        }
        else
        {
            next->setPrev(prev);
        }

        assert(size_ > 0);
        listable->setPrev(NULL);
        listable->setNext(NULL);
        --size_;
    }

private:
    int size_;
    ListableClass* head_;
    ListableClass* tail_;
};
