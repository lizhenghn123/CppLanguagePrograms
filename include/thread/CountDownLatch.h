// ***********************************************************************
// Filename         : CountDownLatch.h
// Author           : LIZHENG
// Created          : 2014-06-08
// Description      : 同步辅助类，在完成一组正在其他线程中执行的操作之前，它允许一个或多个线程一直等待
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-08-25
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_BLOCKINGQUEUE_H
#define ZL_BLOCKINGQUEUE_H
#include "NonCopy.h"
#include "Mutex.h"
#include "Condition.h"
namespace ZL
{
    class CountDownLatch : NonCopy
    {
    public:
        explicit CountDownLatch(int count)
            : count_(count),
              mutex_(),
              condition_(mutex_)
        { }

        void Wait()
        {
            MutexGuard lock(mutex_);
            while(count_ > 0)
            {
                condition_.wait();
            }
        }

        void CountDown()
        {
            MutexGuard lock(mutex_);
            --count_;
            if(count_ == 0)
            {
                condition_.NotifyAll();
            }
        }

        int GetCount() const
        {
            MutexGuard lock(mutex_);
            return count_;
        }

    private:
        int         count_;
        Mutex       mutex_;
        Condition   condition_;
    };

} /* namespace ZL */

#endif /* ZL_BLOCKINGQUEUE_H */