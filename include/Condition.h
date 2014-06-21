// ***********************************************************************
// Filename         : Mutex.h
// Author           : LIZHENG
// Created          : 2014-05-16
// Description      : 条件变量
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-05-16
// 
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_CONTDITION_FILE_H
#define ZL_CONTDITION_FILE_H

#include "osDefine.h"
#include "CriticalSection.h"
#ifdef OS_LINUX
#define LINUX_LOCK
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#elif defined(OS_WINDOWS)
#define WINDOWS_LOCK
#include <Windows.h>
#endif

namespace UTILS
{

class Condition
{
public:
	Condition(UTILS::CriticalSection& cs):m_cs(cs)
	{
	#ifdef OS_LINUX
		pthread_cond_init(&m_condition, NULL);
	#elif defined(OS_WINDOWS)
		InitializeConditionVariable(&m_condition);
	#endif
	}

	~Condition()
	{
	#ifdef OS_LINUX
		pthread_cond_destroy(&m_condition);
	#elif defined(OS_WINDOWS)
	#endif
	}

public:
	void Wait()
	{
	#ifdef OS_LINUX
		pthread_cond_wait(&m_condition, m_cs.GetMutex());
	#elif defined(OS_WINDOWS)
		SleepConditionVariableCS(&m_condition, m_cs.GetMutex(), INFINITE);
	#endif
	}

	void Notify()
	{
	#ifdef OS_LINUX
		pthread_cond_signal(&m_condition);
	#elif defined(OS_WINDOWS)
		WakeConditionVariable(&m_condition);
	#endif
	}

	void NotifyAll()
	{
	#ifdef OS_LINUX
		pthread_cond_broadcast(&m_condition);
	#elif defined(OS_WINDOWS)
		WakeAllConditionVariable(&m_condition);
	#endif
	}

private:
	UTILS::CriticalSection&		m_cs;
#ifdef OS_LINUX
	pthread_cond_t m_condition;
#elif defined(OS_WINDOWS)
	CONDITION_VARIABLE m_condition;
#endif
};

} /* namespace UTILS */

#endif /* ZL_CONTDITION_FILE_H */