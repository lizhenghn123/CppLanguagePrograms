// ***********************************************************************
// Filename         : StopWatch.h
// Author           : LIZHENG
// Created          : 2014-04-28
// Description      :
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-05-14
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************

#ifndef STOPWTACH_FILE_H
#define STOPWTACH_FILE_H

#include "OsDefine.h"

#ifdef OS_WINDOWS
#include <Windows.h>
#include <time.h>
//struct timeval
//{
//	long tv_sec, tv_usec;
//};
#elif defined(OS_LINUX)
#include <sys/time.h>
#else
#error "You must be include OsDefine.h firstly"
#endif

namespace ZL
{

    class StopWatch
    {
    public:
        StopWatch();
        ~StopWatch();
    public:
        bool     Reset();
        timeval  Now();
        double   ElapsedTime();
        double   ElapsedTimeInMill();
        double   ElapsedTimeInMicro();

    private:
        bool Start();
        void GetTimeOfDay(timeval *tv, void *tz);
    private:
        timeval		m_startTime;
    };

} /* namespace UTILS */

#endif /** STOPWTACH_FILE_H */