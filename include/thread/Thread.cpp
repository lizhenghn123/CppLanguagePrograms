#include "Thread.h"
#if defined(OS_WINDOWS)
#include <process.h>
#else
#include <unistd.h>
#endif

NAMESPACE_ZL_START

/// Information to pass to the new thread (what to run).
struct _thread_start_info 
{
	void (*mFunction)(void *); ///< Pointer to the function to be executed.
	void * mArg;               ///< Function argument for the thread function.
	thread * mThread;          ///< Pointer to the thread object.
};

// Thread wrapper function.
#if defined(OS_WINDOWS)
unsigned WINAPI thread::wrapper_function(void * aArg)
#else
void * thread::wrapper_function(void * aArg)
#endif
{
	// Get thread startup information
	_thread_start_info * ti = (_thread_start_info *) aArg;

	try
	{
		// Call the actual client thread function
		ti->mFunction(ti->mArg);
	}
	catch(...)
	{
		// Uncaught exceptions will terminate the application (default behavior
		// according to C++11)
		std::terminate();
	}

	// The thread is no longer executing
	MutexLocker guard(ti->mThread->mDataMutex);
	ti->mThread->mNotAThread = true;

	// The thread is responsible for freeing the startup information
	delete ti;

	return 0;
}

thread::thread(void (*aFunction)(void *), void * aArg)
{
	// Serialize access to this thread structure
	MutexLocker guard(mDataMutex);

	// Fill out the thread startup information (passed to the thread wrapper,
	// which will eventually free it)
	_thread_start_info * ti = new _thread_start_info;
	ti->mFunction = aFunction;
	ti->mArg = aArg;
	ti->mThread = this;

	// The thread is now alive
	mNotAThread = false;

	// Create the thread
#if defined(OS_WINDOWS)
	mHandle = (HANDLE) _beginthreadex(0, 0, wrapper_function, (void *) ti, 0, &mWin32ThreadID);
#elif defined(_TTHREAD_POSIX_)
	if(pthread_create(&mHandle, NULL, wrapper_function, (void *) ti) != 0)
		mHandle = 0;
#endif

	// Did we fail to create the thread?
	if(!mHandle)
	{
		mNotAThread = true;
		delete ti;
	}
}

thread::~thread()
{
	if(joinable())
		std::terminate();
}

void thread::join()
{
	if(joinable())
	{
#if defined(OS_WINDOWS)
		WaitForSingleObject(mHandle, INFINITE);
		CloseHandle(mHandle);
#elif defined(_TTHREAD_POSIX_)
		pthread_join(mHandle, NULL);
#endif
	}
}

bool thread::joinable() const
{
	MutexLocker lock(mDataMutex);
	bool result = !mNotAThread;
	return result;
}

void thread::detach()
{
	MutexLocker lock(mDataMutex);
	if(!mNotAThread)
	{
#if defined(OS_WINDOWS)
		CloseHandle(mHandle);
#elif defined(_TTHREAD_POSIX_)
		pthread_detach(mHandle);
#endif
		mNotAThread = true;
	}
}

thread::id thread::get_id() const
{
	if(!joinable())
		return id();
#if defined(OS_WINDOWS)
	return id((unsigned long int) mWin32ThreadID);
#elif defined(_TTHREAD_POSIX_)
	return _pthread_t_to_ID(mHandle);
#endif
}

unsigned thread::hardware_concurrency()
{
#if defined(OS_WINDOWS)
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return (int) si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
	return (int) sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROC_ONLN)
	return (int) sysconf(_SC_NPROC_ONLN);
#else
	// The standard requires this function to return zero if the number of
	// hardware cores could not be determined.
	return 0;
#endif
}


//------------------------------------------------------------------------------
// this_thread
//------------------------------------------------------------------------------
thread::id this_thread::get_id()
{
#if defined(OS_WINDOWS)
	return thread::id((unsigned long int) GetCurrentThreadId());
#elif defined(_TTHREAD_POSIX_)
	return _pthread_t_to_ID(pthread_self());
#endif
}

NAMESPACE_ZL_END
