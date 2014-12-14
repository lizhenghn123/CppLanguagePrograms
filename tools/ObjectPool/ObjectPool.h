#ifndef ZL_OBJECTPOOL
#define ZL_OBJECTPOOL
#include <list>
#include <vector>
#include "thread/Mutex.h"

template <typename T, class LockType = zl::thread::Mutex>
class ObjectPool
{
public:
    ObjectPool(int preAllocNum = 4)
    {
        allocNum_ = 0;
        pre_alloc(preAllocNum <= 0 ? 1 : preAllocNum);
    }

    ~ObjectPool()
    {
        for(size_t i = 0; i < chunks_.size(); i++)
        {
            delete[] chunks_[i];
        }
    }

public:
    T* alloc()
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
        if(pools_.empty())
        {
            pre_alloc(allocNum_ * 2);
        }

        T *t = pools_.front();
        pools_.pop_front();
        return t;
    }

    void free(T *t)
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
        pools_.push_front(t);
    }

    void pre_alloc(int allocNum)
    {
        T *chunk = new T[allocNum];
        chunks_.push_back(chunk);

        for(int i = 0; i < allocNum; i++)
        {
            T *t = &chunk[i];
            pools_.push_back(t);
        }

        allocNum_ += allocNum;
    }

private:
    LockType         mutex_;
    int              allocNum_;
    std::list<T*>    pools_;
    std::vector<T*>  chunks_;   // save all memory from "new"
};

#endif  /* ZL_OBJECTPOOL */