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
			queue_.push(i);
		}
	}
	void JoinAll()
	{
		running_ = false;
		latch_.wait();
	}
private:
	void ThreadFunction()
	{
		while(running_)
		{
			int one = queue_.pop();
			char result[256];
			_snprintf(result, sizeof(result), "thread_%d_get_%d", this_thread::get_id(), one);
			std::cout << result << "\n";
		}
		latch_.countDown();
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
        queue.push(1);
        queue.push(3);
        queue.push(2);
        queue.push(0);

        int p = -1;
        while (true)
        {
            if (queue.try_pop(p))
                std::cout << p << "\n";
            else
                break;
        }
        std::cout << "================\n";
    }
    {
        zl::thread::BlockingQueue<int, std::stack<int>, zl::thread::tagFILO> queue;
        queue.push(1);
        queue.push(3);
        queue.push(2);
        queue.push(0);

        int p = -1;
        while (true)
        {
            if (queue.try_pop(p))
                std::cout << p << "\n";
            else
                break;
        }
        std::cout << "================\n";
    }
    {
        zl::thread::BlockingQueue<int, std::priority_queue<int>, zl::thread::tagPRIO> queue;
        //zl::BlockingQueue<int> queue;
        queue.push(1);
        queue.push(3);
        queue.push(2);
        queue.push(0);

        int p = -1;
        while (true)
        {
            if (queue.try_pop(p))
                std::cout << p << "\n";
            else
                break;
        }
        std::cout << "================\n";
    }
    {
        zl::thread::BlockingQueue<int> queue;

        for (int i = 0; i<100; ++i)
             queue.push(i);
        std::cout << queue.pop() << "\n";

        std::vector<int> vec;
        if(queue.pop(vec, 10))
             std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
        cout << "\n";
        std::cout << queue.pop() << "\n";
        std::cout << "current size " << queue.size() << "\n";
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
		 queue.push(1);
		 queue.push(3);
		 queue.push(2);
		 queue.push(0); // block, queue.Size() > 3;
	 }
}

int main()
{
    zl::thread::BlockingQueue<int> queue;
    queue.push(1);
    queue.push(3);
    queue.push(2);
    queue.push(0);
    vector<int> vec;
    queue.pop(vec,2);

    test_blockingqueue();

	test_boundleblockingqueue();

    system("pause");
    return 0;
}

