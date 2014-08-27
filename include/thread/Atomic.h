// ***********************************************************************
// Filename         : Atomic.h
// Author           : LIZHENG
// Created          : 2014-08-21
// Description      : 原子操作计数器, 不建议在Windows平台下使用（未测试）
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-08-21
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_ATOMIC_H
#define ZL_ATOMIC_H
#include "OsDefine.h"
#include "Mutex.h"
#ifdef OS_LINUX
#include <pthread.h>
#include <unistd.h>  //for usleep
#include <assert.h>
#include <stdlib.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <linux/types.h>
#include <sys/time.h>

#define ATOMIC_ADD(ptr, v)            __sync_add_and_fetch(ptr, v)
#define ATOMIC_SUB(ptr, v)            __sync_sub_and_fetch(ptr, v)
#define ATOMIC_ADD_AND_FETCH(ptr, v)  __sync_add_and_fetch(ptr, v)
#define ATOMIC_SUB_AND_FETCH(ptr, v)  __sync_sub_and_fetch(ptr, v)
#define ATOMIC_FETCH_AND_ADD(ptr, v)  __sync_fetch_and_add(ptr, v)
#define ATOMIC_FETCH_AND_SUB(ptr, v)  __sync_fetch_and_sub(ptr, v)
#define ATOMIC_FETCH(ptr)             __sync_add_and_fetch(ptr, 0)
#define ATOMIC_SET(ptr, v)            (void)__sync_bool_compare_and_swap(ptr, *(ptr), v)

#elif defined(OS_WINDOWS)
#include <Windows.h>
#define ATOMIC_ADD(ptr, v)            ::InterlockedIncrement(ptr)
#define ATOMIC_SUB(ptr, v)            ::InterlockedDecrement(ptr)
#endif

namespace ZL
{
    class Atomic
    {
    public:
        typedef  volatile long atomic_t;
    public:
        Atomic() : atomic_(0)
        {}
        ~Atomic()
        {}
	public:
        inline void Inc(int n = 1)
        {
            ATOMIC_ADD(&atomic_, n);
        }
        inline atomic_t IncAndFetch(int n = 1)
        {
#ifdef OS_LINUX
            return ATOMIC_ADD_AND_FETCH(&atomic_, n);
#else
            MutexGuard lock(mutex_);
            atomic_ += n;
            return atomic_;
#endif
        }
        inline atomic_t FetchAndInc(int n = 1)
        {
#ifdef OS_LINUX
            return ATOMIC_ADD_AND_FETCH(&atomic_, n);
#else
            MutexGuard lock(mutex_);
            atomic_t old = atomic_;
            atomic_ += n;
            return old;
#endif
        }
        inline void Dec(int n = 1)
        {
            ATOMIC_SUB(&atomic_, n);
        }
        inline atomic_t DecAndFetch(int n = 1)
        {
#ifdef OS_LINUX
            return ATOMIC_SUB_AND_FETCH(&atomic_, n);
#else
            MutexGuard lock(mutex_);
            atomic_ -= n;
            return atomic_;
#endif
        }
        inline atomic_t FetchAndDec(int n = 1)
        {
#ifdef OS_LINUX
            return ATOMIC_FETCH_AND_SUB(&atomic_, n);
#else
            MutexGuard lock(mutex_);
            atomic_t old = atomic_;
            atomic_ -= n;
            return old;
#endif
        }
        inline bool DecAndCheckZero(int n = 1)
        {
#ifdef OS_LINUX
			return 0 == ATOMIC_SUB_AND_FETCH(&atomic_, n);
#else
			MutexGuard lock(mutex_);
			atomic_ -= n;
			return 0 == atomic_;
#endif
        }
        inline atomic_t Value()
        {
            return IncAndFetch(0);
        }
    public:
        atomic_t operator++()
        {
            return ATOMIC_ADD(&atomic_);
        }
        atomic_t operator--()
        {
            return ATOMIC_SUB(&atomic_);
        }
        atomic_t operator++(int)
        {
            return FetchAndInc(1);
        }
        atomic_t operator--(int)
        {
            return FetchAndDec(1);
        }
        atomic_t operator+=(int num)
        {
            return IncAndFetch(num);
        }
        atomic_t operator-=(int num)
        {
            return DecAndFetch(num);
        }
        bool operator=(long lValue)
        {
            return (atomic_ = lValue);
        }
		bool operator==(long value)
		{
			return (atomic_ = value);
		}
        operator long() const
        {
            return atomic_;
        }

    private:
        atomic_t atomic_;
#ifdef OS_WINDOWS
        Mutex    mutex_;
#endif
    };
}

#endif /* ZL_ATOMIC_H */