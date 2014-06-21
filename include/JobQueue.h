// ***********************************************************************
// Filename         : JobQueue.h
// Author           : LIZHENG
// Created          : 2014-05-16
// Description      : 线程安全的异步工作队列
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-05-16
// 
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_JOBQUEUE_FILE_H
#define ZL_JOBQUEUE_FILE_H

#include <vector>
#include <queue>
#include "NonCopy.h"
#include "Condition.h"
#include "CriticalSection.h"

template <typename Job>
class JobQueue : private UTILS::NonCopy
{
public:
	typedef Job                                 JobType;
	typedef std::priority_queue<JobType>	    QueueType;

	typedef UTILS::CriticalSection				MutexType;
	typedef UTILS::CriticalSectionLock			LockType;
	typedef UTILS::Condition					ConditionType;

	JobQueue(): m_stoped(false), m_mutex(), m_hasJob(m_mutex) 
	{

	}

	void Push(const JobType& job)
	{
		LockType lock(m_mutex);
		m_jobQueue.push(job);
		m_hasJob.Notify();
	}

	bool Pop(JobType& job)
	{
		LockType lock(m_mutex);
		while (m_jobQueue.empty() && !m_stoped)
		{
			m_hasJob.Wait();
		}
		if (m_stoped)
		{
			return false;
		}
		job = m_jobQueue.top();
		m_jobQueue.pop();
		return true;
	}

	void Stop()
	{
		m_stoped = true;
		m_hasJob.NotifyAll();	//启动pop里的while循环
	}

protected:
	bool             m_stoped;
	MutexType        m_mutex;	//注意m_mutex和m_hasJob顺序，以及上面初始化列表中的顺序！
	ConditionType    m_hasJob;
	QueueType        m_jobQueue;
};

#endif /* ZL_JOBQUEUE_FILE_H */
