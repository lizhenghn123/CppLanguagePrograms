#include <iostream>
#include "thread/Thread.h"
#include "thread/Mutex.h"
#include "thread/Condition.h"
using namespace std;
using namespace ZL;



// Thread local storage variable
#ifndef NO_TLS
thread_local int gLocalVar;
#endif

// Mutex + global count variable
Mutex gMutex;
int gCount;

// Condition variable
Condition gCond(gMutex);

// Thread function: Thread ID
void ThreadIDs(void * aArg)
{
	cout << " My thread id is " << this_thread::get_id() << "." << endl;
}

#ifndef NO_TLS
// Thread function: Thread-local storage
void ThreadTLS(void * aArg)
{
	gLocalVar = 2;
	cout << " My gLocalVar is " << gLocalVar << "." << endl;
}
#endif

// Thread function: Mutex locking
void ThreadLock(void * aArg)
{
	for(int i = 0; i < 10000; ++ i)
	{
		MutexLocker lock(gMutex);
		++ gCount;
	}
}

// Thread function: Condition notifier
void ThreadCondition1(void * aArg)
{
	MutexLocker lock(gMutex);
	-- gCount;
	gCond.NotifyAll();
}

// Thread function: Condition waiter
void ThreadCondition2(void * aArg)
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
void ThreadYield(void * aArg)
{
	// Yield...
	this_thread::yield();
}

// Thread function: Detach
void ThreadDetach(void * aArg)
{
	// We don't do anything much, just sleep a little...
	this_thread::sleep_for(chrono::milliseconds(100));
}

void test_thread()
{
	// Test 1: Show number of CPU cores in the system
	cout << "PART I: Info" << endl;
	cout << " Number of processor cores: " << thread::hardware_concurrency() << endl;

	// Test 2: thread IDs
	cout << endl << "PART II: Thread IDs" << endl;
	{
		// Show the main thread ID
		cout << " Main thread id is " << this_thread::get_id() << "." << endl;

		// Start a bunch of child threads - only run a single thread at a time
		thread t1(ThreadIDs, 0);
		t1.join();
		thread t2(ThreadIDs, 0);
		t2.join();
		thread t3(ThreadIDs, 0);
		t3.join();
		thread t4(ThreadIDs, 0);
		t4.join();
	}

	// Test 3: thread local storage
	cout << endl << "PART III: Thread local storage" << endl;
#ifndef NO_TLS
	{
		// Clear the TLS variable (it should keep this value after all threads are
		// finished).
		gLocalVar = 1;
		cout << " Main gLocalVar is " << gLocalVar << "." << endl;

		// Start a child thread that modifies gLocalVar
		thread t1(ThreadTLS, 0);
		t1.join();

		// Check if the TLS variable has changed
		if (gLocalVar == 1)
			cout << " Main gLocalID was not changed by the child thread - OK!" << endl;
		else
			cout << " Main gLocalID was changed by the child thread - FAIL!" << endl;
	}
#else
	cout << " TLS is not supported on this platform..." << endl;
#endif

	// Test 4: mutex locking
	cout << endl << "PART IV: Mutex locking (100 threads x 10000 iterations)" << endl;
	{
		// Clear the global counter.
		gCount = 0;

		// Start a bunch of child threads
		list<thread *> threadList;
		for (int i = 0; i < 100; ++i)
			threadList.push_back(new thread(ThreadLock, 0));

		// Wait for the threads to finish
		list<thread *>::iterator it;
		for (it = threadList.begin(); it != threadList.end(); ++it)
		{
			thread * t = *it;
			t->join();
			delete t;
		}

		// Check the global count
		cout << " gCount = " << gCount << endl;
	}

	// Test 6: condition variable
	cout << endl << "PART VI: Condition variable (40 + 1 threads)" << endl;
	{
		// Set the global counter to the number of threads to run.
		gCount = 40;

		// Start the waiting thread (it will wait for gCount to reach zero).
		thread t1(ThreadCondition2, 0);

		// Start a bunch of child threads (these will decrease gCount by 1 when they
		// finish)
		list<thread *> threadList;
		for (int i = 0; i < 40; ++i)
			threadList.push_back(new thread(ThreadCondition1, 0));

		// Wait for the waiting thread to finish
		t1.join();

		// Wait for the other threads to finish
		list<thread *>::iterator it;
		for (it = threadList.begin(); it != threadList.end(); ++it)
		{
			thread * t = *it;
			t->join();
			delete t;
		}
	}

	// Test 7: yield
	cout << endl << "PART VII: Yield (40 + 1 threads)" << endl;
	{
		// Start a bunch of child threads
		list<thread *> threadList;
		for (int i = 0; i < 40; ++i)
			threadList.push_back(new thread(ThreadYield, 0));

		// Yield...
		this_thread::yield();

		// Wait for the threads to finish
		list<thread *>::iterator it;
		for (it = threadList.begin(); it != threadList.end(); ++it)
		{
			thread * t = *it;
			t->join();
			delete t;
		}
	}

	// Test 8: sleep
	cout << endl << "PART VIII: Sleep (10 x 100 ms)" << endl;
	{
		// Sleep...
		cout << " Sleeping" << flush;
		for (int i = 0; i < 10; ++i)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			cout << "." << flush;
		}
		cout << endl;
	}

	// Test 9: detach
	cout << endl << "PART IX: Detach" << endl;
	{
		thread t(ThreadDetach, 0);
		//t.detach(); 这个有问题, 因为t释放时，其回调函数并没有结束
		t.join();
		cout << " Detached from thread." << endl;
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

void func_one(void *arg)
{
	//int i = *((int*)arg);
	int i = (int)arg;
	cout << "func_one : " << i << "\n";
}

void func_two(void *arg/*, int j*/)
{
	int i = (int)arg;
	cout << "func_two : " << i << "\n";
}

int main()
{
	//test_thread();
	
	// Test 9: detach,  这个在windows下有问题, 因为t释放时，其回调函数并没有结束
	cout << endl << "PART IX: Detach" << endl;
	{
		thread t(ThreadDetach, 0);
		t.detach();
		this_thread::sleep_for(chrono::milliseconds(100));
		cout << " Detached from thread." << endl;
	}
	{
		TestT test;
		test.num = 101;
		thread t1(exec_testt, &test);
		t1.join();
	}
	{
		int i = 2;
        //thread t1(func_one, (void *)&i);
		thread t1(func_one, (void *)2);
		t1.join();
	}


	system("pause");
	return 0;
}
