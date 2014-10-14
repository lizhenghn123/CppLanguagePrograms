// ***********************************************************************
// Filename         : ThreadGroup.h
// Author           : LIZHENG
// Created          : 2014-10-13
// Description      :
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-10-13
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_THREADGROU_H
#define ZL_THREADGROU_H
#include "Define.h"
#include "NonCopy.h"
#include "thread/Mutex.h"
NAMESPACE_ZL_THREAD_START
class Thread;

class ThreadGroup : NonCopy
{
public:
    ThreadGroup();
    ~ThreadGroup();

public:
    void CreateThread(std::function<void ()> func, int thread_num = 1);
    void AddThread(Thread *thd);
    void RemoveThread(Thread *thd);
    void JoinAll();
    size_t Size() const;
private:
    mutable Mutex         mutex_;
    std::vector<Thread *>  vecThreads_;
};

NAMESPACE_ZL_THREAD_END
#endif  /* ZL_THREADGROU_H */