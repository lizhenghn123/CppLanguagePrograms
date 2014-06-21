// ***********************************************************************
// Filename         : JobWorker.h
// Author           : LIZHENG
// Created          : 2014-05-16
// Description      : 工作调度，工作在异步的工作队列JobQueue上面
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-05-16
// 
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_JOBWORKER_FILE_H
#define ZL_JOBWORKER_FILE_H

#include "JobQueue.h"

template <typename Queue>
class JobWorker
{
public:
	typedef Queue QueueType;
	typedef typename Queue::JobType JobType;
	typedef boost::function<bool(JobType &)> FunctionType;

	template<typename FunctionType>
	JobWorker(QueueType &queue, FunctionType function, int thread_num = 1):
	queue_(queue), function_(function), thread_num_(thread_num)
	{
		
	}

	JobWorker(QueueType &queue, int thread_num = 1):
	queue_(queue), thread_num_(thread_num)
	{
	
	}

	~JobWorker()
	{
		Stop();
	}

	void Start()
	{
		if(threads_.size() > 0) return;

		for(int i = 0; i < thread_num_; ++i)
		{
			threads_.create_thread(boost::bind(&JobWorker::DoWork, this));
		}
	}

	template<typename Func>
	void Start(FunctionType function)
	{
		function_ = function;
		Start();
	}

	void Stop()
	{
		function_ = 0;
		queue_.Stop();
		threads_.interrupt_all();
		threads_.join_all();//阻塞。直到do_work执行退出
	}

private:
	
	void DoWork() //主体。反复检查工作队列数据，只要有数据就处理，知道处理工作完成
	{
		for(;;)
		{
			JobType job;
			bool bret = queue_.Pop(job);
			if(!function_ || !function_(job))
			{
				//break;
				//TODO:
			}
		}
	}

private:
	QueueType	        	&queue_;
	FunctionType		    function_;
	int						thread_num_;
	boost::thread_group		threads_;
};

#endif /* ZL_JOBWORKER_FILE_H */