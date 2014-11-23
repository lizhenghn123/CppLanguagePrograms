#include <iostream>
#include "net/Timer.h"
#include "net/TimerQueue.h"
#include "base/Timestamp.h"
using namespace std;
using namespace zl;
using namespace zl::base;
using namespace zl::net;
//http://www.tuicool.com/articles/MRRfyuY
void print(Timer *self)
{
    Timestamp now = Timestamp::now();
    printf("async timer trigger : print :%s\n", now.toString().c_str());
    printf("hello world\n");
}

void print_num(Timer *self, int num)
{ 
    Timestamp now = Timestamp::now();
    printf("async timer trigger : print_num :%s\n", now.toString().c_str());
    printf("hello world[%d]\n", num);
}

void test_sync_timer()
{
    cout << "-----------test_sync_timer-----------\n";
    Timestamp now = Timestamp::now();
    printf("%s\n", now.toString().c_str());
    Timer t1(NULL, 3000);
    
    t1.wait();
    now = Timestamp::now();
    printf("%s\n", now.toString().c_str());
}

void test_async_timer()
{
    cout << "-----------test_async_timer-----------\n";
    TimerQueue tqueue ;
    Timer t1(&tqueue, 1000);
    Timer t2(&tqueue, 2300);

    Timestamp now = Timestamp::now();
    printf("async timer start : %s\n", now.toString().c_str());

    t1.async_wait(std::bind(print, &t1));
    t2.async_wait(std::bind(print_num, &t2, 123));

    zl::thread::this_thread::sleep_for(zl::thread::chrono::seconds(4));
    printf("================\n");
}

int main()
{
    test_sync_timer();
    test_async_timer();

    TimerQueue tqueue ;

    system("pause");
    return 0;
}