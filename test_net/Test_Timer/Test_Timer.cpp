#include <iostream>
#include "thread/Timer.h"
#include "thread/TimerQueue.h"
using namespace std;
using namespace zl;
using namespace zl::net;

void print()
{
    Timestamp now = Timestamp::now();
    printf("async timer trigger : print :%s\n", now.toString().c_str());
    printf("hello world\n");
}

void print_num(int num)
{ 
    Timestamp now = Timestamp::now();
    printf("async timer trigger : print_num :%s\n", now.toString().c_str());
    printf("hello world[%d]\n", num);
}

void test_sync_timer()
{
    Timestamp now = Timestamp::now();
    printf("%s\n", now.toString().c_str());
    Timer t1(NULL, 3000);
    t1.wait();
    now = Timestamp::now();
    printf("%s\n", now.toString().c_str());
}

void test_async_timer()
{
    TimerQueue tqueue ;
    Timer t1(&tqueue, 3000);
    Timer t2(&tqueue, 2300);

    Timestamp now = Timestamp::now();
    printf("async timer start : %s\n", now.toString().c_str());

    t1.async_wait(print);
    t2.async_wait(std::bind(print_num, 123));

    ::Sleep(300000);
    printf("================\n");
}

int main()
{
    test_sync_timer();
    test_async_timer();

    system("pause");
    return 0;
}