#include <iostream>
#include <fstream>
#include "thread/Thread.h"
#include "thread/Mutex.h"
#include "thread/Condition.h"
#include "thread/ThreadPool.h"
#include "thread/ThreadLocal.h"
#include "thread/CountDownLatch.h"
using namespace std;
using namespace ZL;

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

// Thread local storage variable
#ifndef NO_TLS
thread_local int gLocalVar;
//ThreadLocal<int> gLocalVar;
#endif

// Mutex + global count variable
Mutex gMutex;
int gCount;

// Condition variable
Condition gCond(gMutex);

// Thread function: Thread ID
void ThreadIDs()
{
	cout << " My thread id is " << this_thread::get_id() << "." << "\n";
}

#ifndef NO_TLS
// Thread function: Thread-local storage
void ThreadTLS()
{
	gLocalVar = 2;
	cout << " My gLocalVar is " << gLocalVar << "." << endl;
}
#endif

// Thread function: Mutex locking
void ThreadLock(int aArg)
{
	for(int i = 0; i < 10000; ++ i)
	{
		MutexLocker lock(gMutex);
		++ gCount;
	}
}

// Thread function: Condition notifier
void ThreadCondition1()
{
	MutexLocker lock(gMutex);
	-- gCount;
	gCond.NotifyAll();
}

// Thread function: Condition waiter
void ThreadCondition2()
{
	cout << " Wating..." << flush;
	MutexLocker lock(gMutex);
	while(gCount > 0)
	{
		cout << "." << flush;
		gCond.Wait();
	}
	cout << "." << endl;
}

// Thread function: Yield
void ThreadYield()
{
	// Yield...
	this_thread::yield();
}

// Thread function: Detach
void ThreadDetach(int a)
{
	// We don't do anything much, just sleep a little...
	this_thread::sleep_for(chrono::milliseconds(100));
}

void test_thread1()
{
	// Test 1: Show number of CPU cores in the system
	cout << "PART I: Info" << "\n";
	cout << " Number of processor cores: " << Thread::hardware_concurrency() << "\n";

	// Test 2: thread IDs
	cout << "\n" << "PART II: Thread IDs" << "\n";
	{
		// Show the main thread ID
		cout << " Main thread id is " << this_thread::get_id() << "." << "\n";

		// Start a bunch of child threads - only run a single thread at a time
		Thread t1(ThreadIDs);
		t1.join();
		Thread t2(ThreadIDs);
		t2.join();
		Thread t3(ThreadIDs);
		t3.join();
		Thread t4(ThreadIDs);
		t4.join();
	}

	// Test 3: thread local storage
	cout << "\n" << "PART III: Thread local storage" << "\n";
#ifndef NO_TLS
	{
		// Clear the TLS variable (it should keep this value after all threads are
		// finished).
		gLocalVar = 1;
		cout << " Main gLocalVar is " << gLocalVar << "." << "\n";

		// Start a child thread that modifies gLocalVar
		Thread t1(ThreadTLS);
		t1.join();

		// Check if the TLS variable has changed
		if (gLocalVar == 1)
			cout << " Main gLocalID was not changed by the child thread - OK!" << "\n";
		else
			cout << " Main gLocalID was changed by the child thread - FAIL!" << "\n";
	}
#else
	cout << " TLS is not supported on this platform..." << "\n";
#endif

	// Test 4: mutex locking
	cout << "\n" << "PART IV: Mutex locking (100 threads x 10000 iterations)" << "\n";
	{
		// Clear the global counter.
		gCount = 0;

		// Start a bunch of child threads
		list<Thread *> threadList;
		for (int i = 0; i < 100; ++i)
			threadList.push_back(new Thread(std::bind(ThreadLock, 0)));

		// Wait for the threads to finish
		list<Thread *>::iterator it;
		for (it = threadList.begin(); it != threadList.end(); ++it)
		{
			Thread * t = *it;
			t->join();
			delete t;
		}

		// Check the global count
		cout << " gCount = " << gCount << "\n";
	}

	// Test 6: condition variable
	cout << "\n" << "PART VI: Condition variable (40 + 1 threads)" << "\n";
	{
		// Set the global counter to the number of threads to run.
		gCount = 40;

		// Start the waiting thread (it will wait for gCount to reach zero).
		Thread t1(ThreadCondition2);

		// Start a bunch of child threads (these will decrease gCount by 1 when they
		// finish)
		list<Thread *> threadList;
		for (int i = 0; i < 40; ++i)
			threadList.push_back(new Thread(ThreadCondition1));

		// Wait for the waiting thread to finish
		t1.join();

		// Wait for the other threads to finish
		list<Thread *>::iterator it;
		for (it = threadList.begin(); it != threadList.end(); ++it)
		{
			Thread * t = *it;
			t->join();
			delete t;
		}
	}

	// Test 7: yield
	cout << "\n" << "PART VII: Yield (40 + 1 threads)" << "\n";
	{
		// Start a bunch of child threads
		list<Thread *> threadList;
		for (int i = 0; i < 40; ++i)
			threadList.push_back(new Thread(ThreadYield));

		// Yield...
		this_thread::yield();

		// Wait for the threads to finish
		list<Thread *>::iterator it;
		for (it = threadList.begin(); it != threadList.end(); ++it)
		{
			Thread * t = *it;
			t->join();
			delete t;
		}
	}

	// Test 8: sleep
	cout << "\n" << "PART VIII: Sleep (10 x 100 ms)" << "\n";
	{
		// Sleep...
		cout << " Sleeping" << flush;
		for (int i = 0; i < 10; ++i)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			cout << "." << flush;
		}
		cout << "\n";
	}
}

class  TestT
{
public:
	static void print_hello()
	{
		std::cout << "hello world\n";
	}

	void print_num(int i)
	{
		std::cout << "num :" << num + i << "\n";
	}

	int num ;
};

void exec_testt(void *arg)
{
	TestT *pt = (TestT*) arg;
	TestT::print_hello();
	pt->print_num(6);
	this_thread::sleep_for(chrono::milliseconds(1000));
}

void func()
{
	cout << "func : " << "\n";
}

void func_one(int i)
{
	cout << "func_one : " << i << "\n";
}

void func_two(int i, float j)
{
	cout << "func_two : " << i << "\t" << j << "\n";
}

void test_thread2()
{
	// Test 9: detach,  这个在windows下有问题, 因为t释放时，其回调函数并没有结束
	cout << "\n" << "PART IX: Detach" << "\n";
	{
		Thread t(std::bind(ThreadDetach, 43), "d");
		t.detach();
		this_thread::sleep_for(chrono::milliseconds(100));
		cout << " Detached from thread." << "\n";
	}
	{
		Thread t1(func, "ff");
		//thread t1(func);
		t1.join();
	}
	{
		Thread t1(std::bind(func_one, 43), "f");
		t1.join();
	}
	{
		Thread t1(std::bind(func_two, 43, 123.45), "f");
		t1.join();
	}
	{
		TestT test;
		test.num = 101;
		Thread t1(std::bind(exec_testt, &test));
		t1.join();
	}
}

void test_threadpoll()
{
	ThreadPool pool("ThreadPool");
	pool.Start(10);

	pool.Run(func);
	pool.Run(std::bind(func_one, 12345));
	for (int i = 0; i < 15; ++i)
	{
		pool.Run(std::bind(func_two, i, i * 2.0));
	}
	this_thread::sleep_for(chrono::milliseconds(1000));
	//CountDownLatch latch(1);
	//pool.Run(std::bind(&CountDownLatch::CountDown, &latch));
	//latch.Wait();
	pool.Stop();
}

class TestTLS
{
public:
	TestTLS() 
	{
		num = -1; 
		cout << "TestTLS : [" << this << "] " << num << "\n"; 
	}
	~TestTLS()
	{
		cout << "~TestTLS : [" << this << "] " << num << "\n"; 
	}
	void set(int n) 
	{ num = n; }
	void plus(int n)
	{ num +=n; }
	void print() { cout << "plus : [" << this << "] " << num << "\n"; }
private:
	int num;
};
ThreadLocal<TestTLS> g_tls;

void testTLS()
{
	g_tls->plus(100);
	g_tls->print();
}
void test_threadlocalstroage()
{
	g_tls->set(10);
	g_tls->print();

	Thread t1(testTLS);
	t1.join();

	g_tls->print();
}

int main()
{
	cout << "------------------------------ test_thread1\n";
	//test_thread1();

	cout << "------------------------------ test_thread2\n";
	//test_thread2();

	cout << "------------------------------ test_threadpoll\n";
	//test_threadpoll();

	cout << "------------------------------ test_threadpoll\n";
	test_threadlocalstroage();
	system("pause");
	return 0;
}
