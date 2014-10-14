#include "ThreadGroup.h"
#include "thread/Thread.h"
#include "base/StrUtil.h"
NAMESPACE_ZL_THREAD_START

ThreadGroup::ThreadGroup()
{

}

ThreadGroup::~ThreadGroup()
{
    for(size_t i = 0; i < vecThreads_.size(); ++i)
    {
        delete vecThreads_[i];
    }
    vecThreads_.clear();
}

void ThreadGroup::CreateThread(std::function<void ()> func, int thread_num/* = 1*/)
{
    MutexLocker lock(mutex_);
    for (int i = 0; i < thread_num; ++i)
    {
        std::string thr_name("threadgroup_thread_");
        thr_name += ToStr(i);
        vecThreads_.push_back(new Thread(func, thr_name));
    }
}

void ThreadGroup::AddThread(Thread *thd)
{
    MutexLocker lock(mutex_);
    vecThreads_.push_back(thd);
}

void ThreadGroup::RemoveThread(Thread *thd)
{
    MutexLocker lock(mutex_);
    std::vector<Thread *>::iterator it = std::find(vecThreads_.begin(), vecThreads_.end(), thd);
    if(it != vecThreads_.end())
    {
        vecThreads_.erase(it);
    }
}

void ThreadGroup::JoinAll()
{
    MutexLocker lock(mutex_);
    for_each(vecThreads_.begin(), vecThreads_.end(), std::bind(&Thread::join, std::placeholders::_1));
}

size_t ThreadGroup::Size() const
{
    MutexLocker lock(mutex_);
    return vecThreads_.size();
}

NAMESPACE_ZL_THREAD_END