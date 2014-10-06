// ***********************************************************************
// Filename         : BoundedBlockingQueue.h
// Author           : LIZHENG
// Created          : 2014-06-08
// Description      : 固定大小的同步阻塞队列，可工作于多线程环境下，可用于线程之间数据存取
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-08-25
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_BOUNDEDBLOCKINGQUEUE_H
#define ZL_BOUNDEDBLOCKINGQUEUE_H
#include <queue>
#include <stack>
#include "Mutex.h"
#include "Condition.h"
#include "BlockingQueue.h"

namespace ZL
{
    //struct tagFIFO {};  //先进先出
    //struct tagFILO {};  //先进后出
    //struct tagPRIO {};  //按优先级

    template <typename Job, typename Queue = std::queue<Job>, typename Order = tagFIFO >
    class BoundedBlockingQueue : public ZL::NonCopy
    {
    public:
        typedef Job                                 JobType;
        typedef Queue	                            QueueType;
        typedef ZL::Mutex							MutexType;
        typedef ZL::MutexLocker	         		    LockGuard;
        typedef ZL::Condition            		    ConditionType;

    public:
        explicit BoundedBlockingQueue(int maxSize)
            : stopFlag_(false),
              maxSize_(maxSize),
              mutex_(),
              notEmpty_(mutex_),
              notFull_(mutex_)
        {}
        virtual ~BoundedBlockingQueue()
        {
            Stop();
        }

    public:
        virtual void Push(const JobType& job)
        {
            LockGuard lock(mutex_);
            while(queue_.size() == maxSize_)  // already full
            {
                notFull_.Wait();
            }
            queue_.push(job);
            notEmpty_.NotifyOne();
        }

        virtual bool Pop(JobType& job)
        {
            LockGuard lock(mutex_);
            while(queue_.empty() && !stopFlag_)
            {
                notEmpty_.Wait();
            }
            if(stopFlag_)
            {
                return false;
            }
            PopOne(job, Order());
            notFull_.NotifyOne();
            return true;
        }

        virtual JobType Pop()
        {
            LockGuard lock(mutex_);
            while(queue_.empty() && !stopFlag_)
            {
                notEmpty_.Wait();
            }
            if(stopFlag_)
            {
                return false;
            }
            JobType job;
            PopOne(job, Order());
            notFull_.NotifyOne();
            return job;
        }

        virtual bool TryPop(JobType& job)
        {
            LockGuard lock(mutex_);
            if(queue_.empty() && !stopFlag_)
                return false;
            PopOne(job, Order());
            notFull_.NotifyOne();
            return true;
        }

        virtual void Stop()
        {
            stopFlag_ = true;
            notFull_.NotifyAll();
            notEmpty_.NotifyAll();
        }

        size_t Size()
        {
            LockGuard lock(mutex_);
            return queue_.size();
        }

        bool Empty()
        {
            LockGuard lock(mutex_);
            return queue_.empty();
        }

        bool Full()
        {
            LockGuard lock(mutex_);
            return queue_.size() == maxSize_;
        }

        template < typename Func >
        void Foreach(const Func& func)
        {
            LockGuard lock(mutex_);
            std::for_each(queue_.begin(), queue_.end(), func);
        }

    private:
        template <typename T>
        bool PopOne(JobType& job, T tag);

        template <>
        bool PopOne(JobType& job, tagFIFO tag)
        {
            job = queue_.front();
            queue_.pop();
            return true;
        }

        template <>
        bool PopOne(JobType& job, tagFILO tag)
        {
            job = queue_.top();
            queue_.pop();
            return true;
        }

        template <>
        bool PopOne(JobType& job, tagPRIO tag)
        {
            job = queue_.top();
            queue_.pop();
            return true;
        }

    protected:
        bool             stopFlag_;
        int              maxSize_;
        MutexType        mutex_;
        ConditionType    notEmpty_;
        ConditionType    notFull_;
        QueueType        queue_;
    };

    /* using is not support in VS2010*/
    //template< typename Job>
    //using BoundedBlockingFifoQueue = ZL::BoundedBlockingQueue<Job, std::queue<Job>, ZL::tagFIFO>;

    //template< typename Job>
    //using BoundedBlockingFiloQueue = ZL::BoundedBlockingQueue<Job, std::stack<Job>, ZL::tagFILO>;
    //
    //template< typename Job>
    //using BoundedBlockingPrioQueue = ZL::BoundedBlockingQueue<Job, std::priority_queue<Job>, ZL::tagPRIO>;

} /* namespace ZL */

#endif /* ZL_BOUNDEDBLOCKINGQUEUE_H */