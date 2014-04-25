#ifndef CSTOPWTACH_FILE_H
#define CSTOPWTACH_FILE_H

#include "osDefine.h"

#ifdef OS_WINDOWS
#include <Windows.h>
#include <time.h>
#elif defined(OS_LINUX)
#include <sys/time.h>
#else
#error "You must be include osDefine.h firstly"
#endif

class CStopWatch
{
public:
	CStopWatch();
	~CStopWatch();

public:
	bool Reset();
	timeval Now();
	double ElapsedTime();
	double ElapsedTimeInMill();
	double ElapsedTimeInMicro();

private:
	bool Start();
	void GetTimeOfDay(timeval *tv,void *);

private:
	timeval		m_startTime;
};

#ifdef OS_WINDOWS
class CStopWatchEx
{
public:
	CStopWatchEx()
	{
		::QueryPerformanceFrequency(&m_liPerfFreq);
		Start();
	}

	void Reset()
	{
		LARGE_INTEGER liPerfNow;
		::QueryPerformanceCounter(&liPerfNow);
		m_liPerfStart = liPerfNow;
	}

	long long Now()
	{
		LARGE_INTEGER liPerfNow;
		::QueryPerformanceCounter(&liPerfNow);
		return liPerfNow.QuadPart;
	}

	long long GetFreq()
	{
		return m_liPerfFreq.QuadPart;
	}

	double ElapsedTime()
	{
		LARGE_INTEGER liPerfNow;
		::QueryPerformanceCounter(&liPerfNow);

		return ( (liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1.0 )/m_liPerfFreq.QuadPart ;
	}

	double ElapsedTimeInMill()
	{
		LARGE_INTEGER liPerfNow;
		::QueryPerformanceCounter(&liPerfNow);

		return ( (liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000.0 )/m_liPerfFreq.QuadPart ;
	}
	double ElapsedTimeInMicro()
	{
		LARGE_INTEGER liPerfNow;
		::QueryPerformanceCounter(&liPerfNow);

		return ( (liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000000.0 )/m_liPerfFreq.QuadPart ;
	}
private:
	void Start()
	{
		::QueryPerformanceCounter(&m_liPerfStart);
	}

private:
	LARGE_INTEGER	m_liPerfFreq;	//Conuts per second
	LARGE_INTEGER	m_liPerfStart;	//Starting count
};
#endif


#endif /** CSTOPWTACH_FILE_H */