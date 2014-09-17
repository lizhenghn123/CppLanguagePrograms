// ***********************************************************************
// Filename         : Thread.h
// Author           : LIZHENG
// Created          : 2014-09-04
// Description      : come from http://tinythreadpp.bitsnbites.eu/
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-09-04
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_THREAD_H
#define ZL_THREAD_H
#include "Define.h"
#include "Mutex.h"
NAMESPACE_ZL_START

// HACK: Mac OS X and early MinGW do not support thread-local storage
#if defined(__APPLE__) || (defined(__MINGW32__) && (__GNUC__ < 4))
#define NO_TLS
#endif

#if !defined(_TTHREAD_CPP11_) && !defined(thread_local)
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#define thread_local __thread
#else
#define thread_local __declspec(thread)
#endif
#endif

class thread
{
public:
#if defined(OS_WINDOWS)
    typedef HANDLE native_handle_type;
#else
    typedef pthread_t native_handle_type;
#endif

    class id;

    /// Default constructor.
    /// Construct a @c thread object without an associated thread of execution
    /// (i.e. non-joinable).
    thread() : mHandle(0), mNotAThread(true)
#if defined(OS_WINDOWS)
        , mWin32ThreadID(0)
#endif
    {}

    /// Thread starting constructor.
    /// Construct a @c thread object with a new thread of execution.
    /// @param[in] aFunction A function pointer to a function of type:
    ///          <tt>void fun(void * arg)</tt>
    /// @param[in] aArg Argument to the thread function.
    /// @note This constructor is not fully compatible with the standard C++
    /// thread class. It is more similar to the pthread_create() (POSIX) and
    /// CreateThread() (Windows) functions.
    thread(void (*aFunction)(void *), void *aArg);

    /// Destructor.
    /// @note If the thread is joinable upon destruction, @c std::terminate()
    /// will be called, which terminates the process. It is always wise to do
    /// @c join() before deleting a thread object.
    ~thread();

    /// Wait for the thread to finish (join execution flows).
    /// After calling @c join(), the thread object is no longer associated with
    /// a thread of execution (i.e. it is not joinable, and you may not join
    /// with it nor detach from it).
    void join();

    /// Check if the thread is joinable.
    /// A thread object is joinable if it has an associated thread of execution.
    bool joinable() const;

    /// Detach from the thread.
    /// After calling @c detach(), the thread object is no longer assicated with
    /// a thread of execution (i.e. it is not joinable). The thread continues
    /// execution without the calling thread blocking, and when the thread
    /// ends execution, any owned resources are released.
    void detach();

    /// Return the thread ID of a thread object.
    id get_id() const;

    /// Get the native handle for this thread.
    /// @note Under Windows, this is a @c HANDLE, and under POSIX systems, this
    /// is a @c pthread_t.
    inline native_handle_type native_handle()
    {
        return mHandle;
    }

    /// Determine the number of threads which can possibly execute concurrently.
    /// This function is useful for determining the optimal number of threads to
    /// use for a task.
    /// @return The number of hardware thread contexts in the system.
    /// @note If this value is not defined, the function returns zero (0).
    static unsigned hardware_concurrency();

private:
    native_handle_type mHandle;   ///< Thread handle.
    mutable Mutex mDataMutex;     ///< Serializer for access to the thread private data.
    bool mNotAThread;             ///< True if this object is not a thread of execution.
#if defined(OS_WINDOWS)
    unsigned int mWin32ThreadID;  ///< Unique thread ID (filled out by _beginthreadex).
#endif

    // This is the internal thread wrapper function.
#if defined(OS_WINDOWS)
    static unsigned WINAPI wrapper_function(void *aArg);
#else
    static void *wrapper_function(void *aArg);
#endif
};

/// Thread ID.
/// The thread ID is a unique identifier for each thread.
/// @see thread::get_id()
class thread::id
{
public:
    /// Default constructor.
    /// The default constructed ID is that of thread without a thread of
    /// execution.
    id() : mId(0) {};

    id(unsigned long int aId) : mId(aId) {};

    id(const id& aId) : mId(aId.mId) {};

    inline id& operator=(const id& aId)
    {
        mId = aId.mId;
        return *this;
    }

    inline friend bool operator==(const id& aId1, const id& aId2)
    {
        return (aId1.mId == aId2.mId);
    }

    inline friend bool operator!=(const id& aId1, const id& aId2)
    {
        return (aId1.mId != aId2.mId);
    }

    inline friend bool operator<=(const id& aId1, const id& aId2)
    {
        return (aId1.mId <= aId2.mId);
    }

    inline friend bool operator<(const id& aId1, const id& aId2)
    {
        return (aId1.mId < aId2.mId);
    }

    inline friend bool operator>=(const id& aId1, const id& aId2)
    {
        return (aId1.mId >= aId2.mId);
    }

    inline friend bool operator>(const id& aId1, const id& aId2)
    {
        return (aId1.mId > aId2.mId);
    }

    inline friend std::ostream& operator <<(std::ostream& os, const id& obj)
    {
        os << obj.mId;
        return os;
    }

private:
    unsigned long int mId;
};


// Related to <ratio> - minimal to be able to support chrono.
typedef long long __intmax_t;

/// Minimal implementation of the @c ratio class. This class provides enough
/// functionality to implement some basic @c chrono classes.
template <__intmax_t N, __intmax_t D = 1>
class ratio
{
public:
    static double _as_double()
    {
        return double(N) / double(D);
    }
};

/// Minimal implementation of the @c chrono namespace.
/// The @c chrono namespace provides types for specifying time intervals.
namespace chrono
{
    /// Duration template class. This class provides enough functionality to
    /// implement @c this_thread::sleep_for().
    template <class _Rep, class _Period = ratio<1> > class duration
    {
    private:
        _Rep rep_;
    public:
        typedef _Rep rep;
        typedef _Period period;

        /// Construct a duration object with the given duration.
        template <class _Rep2>
        explicit duration(const _Rep2& r) : rep_(r) {};

        /// Return the value of the duration object.
        rep count() const
        {
            return rep_;
        }
    };

    // Standard duration types.
    typedef duration<__intmax_t, ratio<1, 1000000000> > nanoseconds; ///< Duration with the unit nanoseconds.
    typedef duration<__intmax_t, ratio<1, 1000000> > microseconds;   ///< Duration with the unit microseconds.
    typedef duration<__intmax_t, ratio<1, 1000> > milliseconds;      ///< Duration with the unit milliseconds.
    typedef duration<__intmax_t> seconds;                            ///< Duration with the unit seconds.
    typedef duration<__intmax_t, ratio<60> > minutes;                ///< Duration with the unit minutes.
    typedef duration<__intmax_t, ratio<3600> > hours;                ///< Duration with the unit hours.
}

/// The namespace @c this_thread provides methods for dealing with the
/// calling thread.
namespace this_thread
{
    /// Return the thread ID of the calling thread.
    thread::id get_id();

    /// Yield execution to another thread.
    /// Offers the operating system the opportunity to schedule another thread
    /// that is ready to run on the current processor.
    inline void yield()
    {
#if defined(OS_WINDOWS)
        Sleep(0);
#else
        sched_yield();
#endif
    }

    /// Blocks the calling thread for a period of time.
    /// @param[in] aTime Minimum time to put the thread to sleep.
    /// Example usage:
    /// @code
    /// // Sleep for 100 milliseconds
    /// this_thread::sleep_for(chrono::milliseconds(100));
    /// @endcode
    /// @note Supported duration types are: nanoseconds, microseconds,
    /// milliseconds, seconds, minutes and hours.
    template <class _Rep, class _Period> void sleep_for(const chrono::duration<_Rep, _Period>& aTime)
    {
#if defined(OS_WINDOWS)
        Sleep(int(double(aTime.count()) * (1000.0 * _Period::_as_double()) + 0.5));
#else
        usleep(int(double(aTime.count()) * (1000000.0 * _Period::_as_double()) + 0.5));
#endif
    }
}

NAMESPACE_ZL_END


#endif  /* ZL_THREAD_H */