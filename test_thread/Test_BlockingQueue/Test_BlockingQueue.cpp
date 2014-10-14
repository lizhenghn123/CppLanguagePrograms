#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include "thread/Thread.h"
#include "thread/BlockingQueue.h"
#include "thread/BoundedBlockingQueue.h"
#include "thread/ThreadPool.h"
#include "thread/CountDownLatch.h"
using namespace std;
using namespace zl;
using namespace zl::thread;

//template<class TQueue>
class TestBlockingQueue
{
public:
	TestBlockingQueue(int queue_size, int thread_num)
		: queue_(queue_size), latch_(thread_num)
	{
		for(int i=0; i<thread_num; ++i)
		{
			char name[32];
			_snprintf(name, sizeof(name), "thread_%d", i);
			//Thread *f = new Thread(std::bind(&TestBlockingQueue::ThreadFunction, this), name);
			threads_.push_back(new Thread(std::bind(&TestBlockingQueue::ThreadFunction, this), name));
		}
		running_ = true;
	}
	void Run(int loop)
	{
		for(int i = 0; i<loop; ++i)
		{
			queue_.Push(i);
		}
	}
	void JoinAll()
	{
		running_ = false;
		latch_.Wait();
	}
private:
	void ThreadFunction()
	{
		while(running_)
		{
			int one = queue_.Pop();
			char result[256];
			_snprintf(result, sizeof(result), "thread_%d_get_%d", this_thread::get_id(), one);
			std::cout << result << "\n";
		}
		latch_.CountDown();
	}
private:
	//TQueue  queue_;
	zl::thread::BoundedBlockingQueue<int> queue_;
	std::vector<zl::thread::Thread*> threads_;
	zl::thread::CountDownLatch latch_;
	bool running_;

};

void test_blockingqueue()
{
    {
        zl::thread::BlockingQueue<int, std::queue<int>, zl::thread::tagFIFO> queue;
        queue.Push(1);
        queue.Push(3);
        queue.Push(2);
        queue.Push(0);

        int p = -1;
        while (true)
        {
            if (queue.TryPop(p))
                std::cout << p << "\n";
            else
                break;
        }
        std::cout << "================\n";
    }
    {
        zl::thread::BlockingQueue<int, std::stack<int>, zl::thread::tagFILO> queue;
        queue.Push(1);
        queue.Push(3);
        queue.Push(2);
        queue.Push(0);

        int p = -1;
        while (true)
        {
            if (queue.TryPop(p))
                std::cout << p << "\n";
            else
                break;
        }
        std::cout << "================\n";
    }
    {
        zl::thread::BlockingQueue<int, std::priority_queue<int>, zl::thread::tagPRIO> queue;
        //zl::BlockingQueue<int> queue;
        queue.Push(1);
        queue.Push(3);
        queue.Push(2);
        queue.Push(0);

        int p = -1;
        while (true)
        {
            if (queue.TryPop(p))
                std::cout << p << "\n";
            else
                break;
        }
        std::cout << "================\n";
    }
    {
        zl::thread::BlockingQueue<int> queue;

        for (int i = 0; i<100; ++i)
             queue.Push(i);
        std::cout << queue.Pop() << "\n";

        std::vector<int> vec;
        if(queue.Pop(vec, 10))
             std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
        cout << "\n";
        std::cout << queue.Pop() << "\n";
        std::cout << "current size " << queue.Size() << "\n";
        std::cout << "================\n";
    }
}

void test_boundleblockingqueue()
{
	 TestBlockingQueue tbq(5, 3);
	 tbq.Run(13);
	 this_thread::sleep_for(chrono::milliseconds(1000));
	 tbq.JoinAll();

	 {
		 zl::thread::BoundedBlockingQueue<int, std::priority_queue<int>, zl::thread::tagPRIO> queue(3);
		 queue.Push(1);
		 queue.Push(3);
		 queue.Push(2);
		 queue.Push(0); // block, queue.Size() > 3;
	 }
}

int main()
{
    test_blockingqueue();

	test_boundleblockingqueue();

    system("pause");
    return 0;
}

