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
#ifndef ZL_COUNTDOWNLATCH_H
#define ZL_COUNTDOWNLATCH_H
#include "NonCopy.h"
#include "thread/Mutex.h"
#include "thread/Condition.h"
NAMESPACE_ZL_THREAD_START

class CountDownLatch : NonCopy
{
public:
    explicit CountDownLatch(int count)
        : count_(count),
        mutex_(),
        condition_(mutex_)
    {

    }

    void Wait()
    {
        MutexLocker guard(mutex_);
        while(count_ > 0)
        {
            condition_.Wait();
        }
    }

    void CountDown()
    {
        MutexLocker guard(mutex_);
        --count_;
        if(count_ == 0)
        {
            condition_.NotifyAll();
        }
    }

    int GetCount() const
    {
        MutexLocker guard(mutex_);
        return count_;
    }

private:
    int              count_;
    mutable Mutex    mutex_;
    Condition        condition_;
};

NAMESPACE_ZL_THREAD_END

#endif /* ZL_BLOCKINGQUEUE_H */