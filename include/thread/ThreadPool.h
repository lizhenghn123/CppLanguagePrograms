// ***********************************************************************
// Filename         : Threadpool.h
// Author           : LIZHENG
// Created          : 2014-10-06
// Description      :
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-10-06
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_THREADPOOL_H
#define ZL_THREADPOOL_H
#include "Define.h"
#include "NonCopy.h"
#include "Mutex.h"
#include "Condition.h"
NAMESPACE_ZL_START

class Thread;

class ThreadPool : NonCopy
{
public:
    //typedef std::function<bool ()> Task;
    typedef std::function<void ()> Task;

public:
    explicit ThreadPool(const std::string& name = "ThreadPool");
    ~ThreadPool();

public:
    void Start(int numThreads);
    void Stop();

    void Run(const Task& f);

private:
    void ExecuteThread();
    Task PopOne();

private:
    std::string           name_;
    bool                  running_;
    Mutex                 mutex_;
    Condition             cond_;
    std::deque<Task>      queue_;
    std::vector<Thread *>  threads_;
};

NAMESPACE_ZL_END
#endif  /* ZL_THREADPOOL_H */