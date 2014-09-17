// ***********************************************************************
// Filename         : BlockingQueue.h
// Author           : LIZHENG
// Created          : 2014-06-08
// Description      : 同步阻塞队列，可工作于多线程环境下，可用于线程之间数据存取
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-08-25
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_BLOCKINGQUEUE_H
#define ZL_BLOCKINGQUEUE_H
#include <queue>
#include <stack>
#include "Mutex.h"
#include "Condition.h"

namespace ZL
{

    struct tagFIFO {};  //先进先出
    struct tagFILO {};  //先进后出
    struct tagPRIO {};  //按优先级

    template <typename Job, typename Queue = std::queue<Job>, typename Order = tagFIFO >
    class BlockingQueue : public ZL::NonCopy
    {
    public:
        typedef Job                                 JobType;
        typedef Queue	                            QueueType;
        typedef ZL::Mutex							MutexType;
        typedef ZL::MutexLocker	         		    LockGuard;
        typedef ZL::Condition            		    ConditionType;

    public:
        BlockingQueue() : stopFlag_(false), mutex_(), has_job_(mutex_) {}
        virtual ~BlockingQueue()
        {
            Stop();
        }

    public:
        virtual void Push(const JobType& job)
        {
            LockGuard lock(mutex_);
            queue_.push(job);
            has_job_.NotifyOne();
        }

        virtual bool Pop(JobType& job)
        {
            LockGuard lock(mutex_);
            while(queue_.empty() && !stopFlag_)
            {
                has_job_.Wait();
            }
            if(stopFlag_)
            {
                return false;
            }

            return PopOne(job, Order());
        }

        virtual JobType Pop()
        {
            LockGuard lock(mutex_);
            while(queue_.empty() && !stopFlag_)
            {
                has_job_.Wait();
            }
            if(stopFlag_)
            {
                return false;
            }

            JobType job;
            PopOne(job, Order());
            return job;
        }

        virtual bool TryPop(JobType& job)
        {
            LockGuard lock(mutex_);

            if(queue_.empty() && !stopFlag_)
                return false;

            return PopOne(job, Order());
        }

        virtual void Stop()
        {
            stopFlag_ = true;
            has_job_.NotifyAll();
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
        MutexType        mutex_;
        ConditionType    has_job_;
        QueueType        queue_;
    };

    /* using is not support in VS2010*/
    //template< typename Job>
    //using FifoJobQueue = ZL::BlockingQueue<Job, std::queue<Job>, tagFIFO>;

    //template< typename Job>
    //using FiloJobQueue = ZL::BlockingQueue<Job, std::stack<Job>, tagFILO>;
    //
    //template< typename Job>
    //using PrioJobQueue = ZL::BlockingQueue<Job, std::priority_queue<Job>, tagPRIO>;

} /* namespace ZL */

#endif /* ZL_BLOCKINGQUEUE_H */