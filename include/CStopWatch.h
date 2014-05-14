// ***********************************************************************
// Filename         : CStopWatch.h
// Author           : Administrator
// Created          : 2014-04-28
// Description      : 
//
// Last Modified By : Administrator
// Last Modified On : 2014-05-14
// 
// Copyright (c) 北京大学图形学图形学与人机交互人机交互实验室. All rights reserved.
// ***********************************************************************

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

#endif /** CSTOPWTACH_FILE_H */