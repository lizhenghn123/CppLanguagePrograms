#include <iostream>
#include <vector>
#include "thread/BlockingQueue.h"
#include "thread/BoundedBlockingQueue.h"
using namespace std;

void test_block_queue()
{
    {
        ZL::BlockingQueue<int> queue;
        queue.Push(1);
        queue.Push(3);
        queue.Push(2);
        queue.Push(0);
        std::cout << queue.Size() << "\t" << queue.Empty() << "\n";
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
        ZL::BlockingQueue<int, std::stack<int>, ZL::tagFILO> queue;
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
        ZL::BlockingQueue<int, std::priority_queue<int>, ZL::tagPRIO > queue;
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
}

void test_bounded_block_queue()
{
    {
        ZL::BoundedBlockingQueue<int> queue(5);
        queue.Push(1);
        queue.Push(3);
        queue.Push(2);
        queue.Push(0);
        std::cout << queue.Size() << "\t" << queue.Empty() << "\n";
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
        ZL::BoundedBlockingQueue<int, std::stack<int>, ZL::tagFILO> queue(30);
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
        ZL::BoundedBlockingQueue<int, std::priority_queue<int>, ZL::tagPRIO> queue(3);
        queue.Push(1);
        queue.Push(3);
        queue.Push(2);
        queue.Push(0); // block, queue.Size() > 3;
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
}

int main()
{
    std::cout << "----------------------test_block_queue----------------------\n";
    test_block_queue();
    std::cout << "----------------------test_bounded_block_queue----------------------\n";
    test_bounded_block_queue();
    std::cout << "OK\n";
    getchar();
    return 0;
}