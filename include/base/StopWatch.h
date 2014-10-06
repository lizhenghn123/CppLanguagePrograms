// ***********************************************************************
// Filename         : StopWatch.h
// Author           : LIZHENG
// Created          : 2014-04-28
// Description      : 时间统计工具
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


//实现
namespace ZL
{
	StopWatch::StopWatch()
	{
		Start();
	}

	StopWatch::~StopWatch()
	{
	}

	bool StopWatch::Start()
	{
		return Reset();
	}

	bool StopWatch::Reset()
	{
		GetTimeOfDay(&m_startTime, NULL);
		return true;
	}

	timeval StopWatch::Now()
	{
		timeval now;
		GetTimeOfDay(&now, NULL);
		return now;
	}

	double StopWatch::ElapsedTime()
	{
		timeval now;
		GetTimeOfDay(&now, NULL);

		return 1.0 * (now.tv_sec - m_startTime.tv_sec) + 1.0 * (now.tv_usec - m_startTime.tv_usec) / 1000000;
	}

	double StopWatch::ElapsedTimeInMill()
	{
		timeval now;
		GetTimeOfDay(&now, NULL);

		return (now.tv_sec - m_startTime.tv_sec) * 1000 + 1.0 * (now.tv_usec - m_startTime.tv_usec) / 1000;
	}

	double StopWatch::ElapsedTimeInMicro()
	{
		timeval now;
		GetTimeOfDay(&now, NULL);

		return (now.tv_sec - m_startTime.tv_sec) * 1000000 + 1.0 * (now.tv_usec - m_startTime.tv_usec);
	}

	void StopWatch::GetTimeOfDay(timeval *tv, void *tz)
	{
#ifdef OS_LINUX
		gettimeofday(tv, tz);
#elif defined(OS_WINDOWS)
		typedef unsigned __int64 uint64;
#define EPOCHFILETIME (116444736000000000ULL)
		FILETIME ft;
		LARGE_INTEGER li;
		uint64 tt;

		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		tt = (li.QuadPart - EPOCHFILETIME) / 10;
		tv->tv_sec = tt / 1000000;
		tv->tv_usec = tt % 1000000;

		////以下计算精度不够
		//time_t clock;
		//struct tm tm;
		//SYSTEMTIME wtm;
		//
		//GetLocalTime(&wtm);
		//tm.tm_year     = wtm.wYear - 1900;
		//tm.tm_mon     = wtm.wMonth - 1;
		//tm.tm_mday     = wtm.wDay;
		//tm.tm_hour     = wtm.wHour;
		//tm.tm_min     = wtm.wMinute;
		//tm.tm_sec     = wtm.wSecond;
		//tm. tm_isdst    = -1;
		//clock = mktime(&tm);
		//tv->tv_sec = clock;
		//tv->tv_usec = wtm.wMilliseconds * 1000;
#endif
	}
}
#endif /** STOPWTACH_FILE_H */