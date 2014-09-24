// ***********************************************************************
// Filename         : ProcessTimeCounter.h
// Author           : LIZHENG
// Created          : 2014-09-24
// Description      : 与进程相关的性能统计工具
//                  
// Last Modified By : LIZHENG
// Last Modified On : 2014-09-24
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#include "OsDefine.h"
#include <assert.h>
#ifdef OS_LINUX
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#else
#include <Windows.h>
#endif

class ProcessTimeCounter
{
public:
#ifdef OS_LINUX
	typedef int64_t             interval_type;
#else
	typedef __int64             interval_type;
#endif

public:
	ProcessTimeCounter();

public:
	void        Start();
	void        Stop();

public:
	// Kernel
	/// \brief The elapsed count in the measurement period for kernel mode activity
	///
	/// This represents the extent, in machine-specific increments, of the measurement period for kernel mode activity
	interval_type   KernelPeriodCount() const;
	/// \brief The number of whole seconds in the measurement period for kernel mode activity
	///
	/// This represents the extent, in whole seconds, of the measurement period for kernel mode activity
	interval_type   KernelSeconds() const;
	/// \brief The number of whole milliseconds in the measurement period for kernel mode activity
	///
	/// This represents the extent, in whole milliseconds, of the measurement period for kernel mode activity
	interval_type   KernelMillSeconds() const;
	/// \brief The number of whole microseconds in the measurement period for kernel mode activity
	///
	/// This represents the extent, in whole microseconds, of the measurement period for kernel mode activity
	interval_type   KernelMicroseconds() const;

	// User
	/// \brief The elapsed count in the measurement period for user mode activity
	///
	/// This represents the extent, in machine-specific increments, of the measurement period for user mode activity
	interval_type   UserPeriodCount() const;
	/// \brief The number of whole seconds in the measurement period for user mode activity
	///
	/// This represents the extent, in whole seconds, of the measurement period for user mode activity
	interval_type   UserSeconds() const;
	/// \brief The number of whole milliseconds in the measurement period for user mode activity
	///
	/// This represents the extent, in whole milliseconds, of the measurement period for user mode activity
	interval_type   UserMillSeconds() const;
	/// \brief The number of whole microseconds in the measurement period for user mode activity
	///
	/// This represents the extent, in whole microseconds, of the measurement period for user mode activity
	interval_type   UserMicroSeconds() const;

	// Total
	/// \brief The elapsed count in the measurement period
	///
	/// This represents the extent, in machine-specific increments, of the measurement period
	interval_type   PeriodCount() const;
	/// \brief The number of whole seconds in the measurement period
	///
	/// This represents the extent, in whole seconds, of the measurement period
	interval_type   Seconds() const;
	/// \brief The number of whole milliseconds in the measurement period
	///
	/// This represents the extent, in whole milliseconds, of the measurement period
	interval_type   MillSeconds() const;
	/// \brief The number of whole microseconds in the measurement period
	///
	/// This represents the extent, in whole microseconds, of the measurement period
	interval_type   MicroSeconds() const;

private:
#ifdef OS_LINUX
	typedef struct timeval timeval_t;
#else
	typedef __int64        timeval_t;
	static HANDLE   ProcessHandle();
#endif
	timeval_t   m_kernelStart;
	timeval_t   m_kernelEnd;
	timeval_t   m_userStart;
	timeval_t   m_userEnd;
};

inline ProcessTimeCounter::ProcessTimeCounter()
{
	// Note that the constructor does nothing, for performance reasons. Calling
	// any of the Attribute methods before having gone through a start()-stop()
	// cycle will yield undefined results.
}

#if defined(OS_LINUX)
inline void ProcessTimeCounter::Start()
{
	struct rusage   r_usage;

	::getrusage(RUSAGE_SELF, &r_usage);

	m_kernelStart   =   r_usage.ru_stime;
	m_userStart     =   r_usage.ru_utime;
}

inline void ProcessTimeCounter::Stop()
{
	struct rusage   r_usage;

	::getrusage(RUSAGE_SELF, &r_usage);

	m_kernelEnd     =   r_usage.ru_stime;
	m_userEnd       =   r_usage.ru_utime;
}

// Kernel
inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelPeriodCount() const
{
	return KernelMicroseconds();
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelSeconds() const
{
	assert("end before start: stop() must be called after start()" && m_kernelStart.tv_sec <= m_kernelEnd.tv_sec);

	long    secs    =   m_kernelEnd.tv_sec - m_kernelStart.tv_sec;
	long    usecs   =   m_kernelEnd.tv_usec - m_kernelStart.tv_usec;

	assert(usecs >= 0 || secs > 0);

	return secs + usecs / (1000 * 1000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelMillSeconds() const
{
	assert("end before start: stop() must be called after start()" && m_kernelStart.tv_sec <= m_kernelEnd.tv_sec);

	long    secs    =   m_kernelEnd.tv_sec - m_kernelStart.tv_sec;
	long    usecs   =   m_kernelEnd.tv_usec - m_kernelStart.tv_usec;

	assert(usecs >= 0 || secs > 0);

	return secs * 1000 + usecs / 1000;
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelMicroseconds() const
{
	assert("end before start: stop() must be called after start()" && m_kernelStart.tv_sec <= m_kernelEnd.tv_sec);

	long    secs    =   m_kernelEnd.tv_sec - m_kernelStart.tv_sec;
	long    usecs   =   m_kernelEnd.tv_usec - m_kernelStart.tv_usec;

	assert(usecs >= 0 || secs > 0);

	return secs * (1000 * 1000) + usecs;
}

// User
inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserPeriodCount() const
{
	return UserMicroSeconds();
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserSeconds() const
{
	assert("end before start: stop() must be called after start()" && m_userStart.tv_sec <= m_userEnd.tv_sec);

	long    secs    =   m_userEnd.tv_sec - m_userStart.tv_sec;
	long    usecs   =   m_userEnd.tv_usec - m_userStart.tv_usec;

	assert(usecs >= 0 || secs > 0);

	return secs + usecs / (1000 * 1000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserMillSeconds() const
{
	assert("end before start: stop() must be called after start()" &&  m_userStart.tv_sec <= m_userEnd.tv_sec);

	long    secs    =   m_userEnd.tv_sec - m_userStart.tv_sec;
	long    usecs   =   m_userEnd.tv_usec - m_userStart.tv_usec;

	assert(usecs >= 0 || secs > 0);

	return secs * 1000 + usecs / 1000;
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserMicroSeconds() const
{
	assert("end before start: stop() must be called after start()" &&  m_userStart.tv_sec <= m_userEnd.tv_sec);

	long    secs    =   m_userEnd.tv_sec - m_userStart.tv_sec;
	long    usecs   =   m_userEnd.tv_usec - m_userStart.tv_usec;

	assert(usecs >= 0 || secs > 0);

	return secs * (1000 * 1000) + usecs;
}

// Total
inline ProcessTimeCounter::interval_type ProcessTimeCounter::PeriodCount() const
{
	return KernelPeriodCount() + UserPeriodCount();
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::Seconds() const
{
	return PeriodCount() / interval_type(10000000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::MillSeconds() const
{
	return PeriodCount() / interval_type(10000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::MicroSeconds() const
{
	return PeriodCount() / interval_type(10);
}
#elif defined(OS_WINDOWS)
inline /* static */ HANDLE ProcessTimeCounter::ProcessHandle()
{
	static HANDLE   s_hProcess = ::GetCurrentProcess();
	return s_hProcess;
}

inline void ProcessTimeCounter::Start()
{
	FILETIME    creationTime;
	FILETIME    exitTime;

	::GetProcessTimes(ProcessHandle(), &creationTime, &exitTime, reinterpret_cast<LPFILETIME>(&m_kernelStart), reinterpret_cast<LPFILETIME>(&m_userStart));
}

inline void ProcessTimeCounter::Stop()
{
	FILETIME    creationTime;
	FILETIME    exitTime;

	::GetProcessTimes(ProcessHandle(), &creationTime, &exitTime, reinterpret_cast<LPFILETIME>(&m_kernelEnd), reinterpret_cast<LPFILETIME>(&m_userEnd));
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelPeriodCount() const
{
	return static_cast<interval_type>(m_kernelEnd - m_kernelStart);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelSeconds() const
{
	return KernelPeriodCount() / interval_type(10000000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelMillSeconds() const
{
	return KernelPeriodCount() / interval_type(10000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::KernelMicroseconds() const
{
	return KernelPeriodCount() / interval_type(10);
}

// User
inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserPeriodCount() const
{
	return static_cast<interval_type>(m_userEnd - m_userStart);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserSeconds() const
{
	return UserPeriodCount() / interval_type(10000000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserMillSeconds() const
{
	return UserPeriodCount() / interval_type(10000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::UserMicroSeconds() const
{
	return UserPeriodCount() / interval_type(10);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::PeriodCount() const
{
	return KernelPeriodCount() + UserPeriodCount();
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::Seconds() const
{
	return PeriodCount() / interval_type(10000000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::MillSeconds() const
{
	return PeriodCount() / interval_type(10000);
}

inline ProcessTimeCounter::interval_type ProcessTimeCounter::MicroSeconds() const
{
	return PeriodCount() / interval_type(10);
}
#endif