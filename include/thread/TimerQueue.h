// ***********************************************************************
// Filename         : TimerQueue.h
// Author           : LIZHENG
// Created          : 2014-10-126
// Description      :
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-10-26
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_TIMERQUEUE_H
#define ZL_TIMERQUEUE_H
//#include <WinSock2.h>
#include "Define.h"
#include "base/NonCopy.h"
#include "thread/Mutex.h"
#include "thread/Thread.h"
NAMESPACE_ZL_THREAD_START

class Timer;

class TimerQueue
{
public:    
    static TimerQueue * instance();
    TimerQueue();
    ~TimerQueue()
    {

    }
    void Start();   
    void Stop();  
    void AddTimer(Timer * vtimer);   
    void DeleteTimer(Timer * vtimer); 

private:    
    void AddTimer_(Timer * vtimer); 
    void DeleteTimer_(Timer * vtimer); 

private:
     void ProcessThread(); //定时器延迟线程

    static zl::thread::Mutex m_mutex;
    static TimerQueue * m_instance;

    bool running_;
    Thread thread_;
    std::list<Timer*> timers_;
};

NAMESPACE_ZL_THREAD_END
#endif  /* ZL_TIMERQUEUE_H */