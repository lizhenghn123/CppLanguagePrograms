// ***********************************************************************
// Filename         : CriticalSection.h
// Author           : LIZHENG
// Created          : 2013-08-19
// Description      : 临界区 及 临界区锁
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-05-16
// 
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_CRITICALSECTION_FILE_H
#define	ZL_CRITICALSECTION_FILE_H

#include <iostream>
#include "osDefine.h"

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
	class CriticalSection
	{
	public:
		CriticalSection() { Init_(); }
		~CriticalSection() { Close_(); }

	public:
		void Lock()
		{
		#ifdef	WINDOWS_LOCK
			EnterCriticalSection( &m_mutex );
		#else
			//pthread_mutex_t pmutex = PTHREAD_MUTEX_INITIALIZER;
			if(pthread_mutex_lock(&m_mutex)==0)
			{
                            //std::cout<<"pthread_mutex_lock\n";
			}
			else
			{
				// std::cout<<"lock mutex fail\n";
			}
		#endif
		}

		bool TryLock() //const
		{
		#ifdef	WINDOWS_LOCK
			#if(_WIN32_WINNT >= 0x0400)
				return TryEnterCriticalSection( &m_mutex ) != 0;
			#else
				return false;
			#endif
		#else
			int ret=pthread_mutex_trylock(&m_mutex);
			if( ret !=0 )
			{
				std::cout<<"pthread_mutex_trylock fail,code = "<<ret<<"\n";
				return false;
			}
			return true;
		#endif
		}

		void Unlock() 
		{
		#ifdef	WINDOWS_LOCK
			LeaveCriticalSection( &m_mutex );
		#else
			if(pthread_mutex_unlock(&m_mutex)==0)
                        {
				//std::cout<<"pthread_mutex_unlock\n";
			}
			else
                        {
				//std::cout<<"unlock mutex fail\n";
                        }
		#endif
		}

		#ifdef	WINDOWS_LOCK
		CRITICAL_SECTION*	GetMutex()
		#else
		pthread_mutex_t*	GetMutex()
		#endif
		{
			return &m_mutex;
		}
		
	private:
		void Init_()
		{
		#ifdef	WINDOWS_LOCK
			#if (_WIN32_WINNT >= 0x0403)
				if ( !InitializeCriticalSectionAndSpinCount( &m_mutex, 0x80000400 ) )
			#endif /* _WIN32_WINNT >= 0x0403 */
				InitializeCriticalSection( &m_mutex );
		#else
			pthread_mutex_init(&m_mutex,NULL);
		#endif
		}

		void Close_()
		{
		#ifdef	WINDOWS_LOCK
			DeleteCriticalSection( &m_mutex );
		#else
			pthread_mutex_destroy(&m_mutex);
		#endif
		}

	private:
	#ifdef	WINDOWS_LOCK
		mutable CRITICAL_SECTION m_mutex;
	#else
		pthread_mutex_t m_mutex ;//= PTHREAD_MUTEX_INITIALIZER;
	#endif
	};

	class CriticalSectionLock
	{
	public:
		explicit CriticalSectionLock(CriticalSection & rcs ) :cs(rcs) { cs.Lock(); }
		~CriticalSectionLock() { cs.Unlock(); }
	private:
		CriticalSectionLock(const CriticalSectionLock&);
		CriticalSectionLock& operator=(const CriticalSectionLock&);

	private:
		CriticalSection & cs;
	};
}

#endif	/* CRITICALSECTION_H */