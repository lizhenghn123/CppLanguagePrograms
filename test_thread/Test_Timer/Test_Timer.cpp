#include <iostream>
#include "thread/Timer.h"
#include "thread/TimerQueue.h"
using namespace std;
using namespace zl;
using namespace zl::thread;

void func(Timer * timer, void *data)
{
    printf("hi,%d\n",*((int*)(data)));
}

int main()
{
    {
        TimerQueue::instance()->start();
        int i = 1, j=2, m=3, n=4;
        Timer a(1000,func,(void *)&i,Timer::TIMER_CIRCLE);
        Timer a1(2000,func,(void *)&j,Timer::TIMER_ONCE);
        Timer a2(3000,func,(void *)&m,Timer::TIMER_ONCE);
        Timer a3(1000,func,(void *)&n,Timer::TIMER_CIRCLE);
        Sleep(2000);
        a.start();
        a1.start();
        a2.start();
        a3.start();
        //a.Start();
        //a1.Start();
        //a2.Start();
        //a3.Start();

        a.reset(2000);
        a1.stop();
        a3.stop();
        //while(1);
        Sleep(20000);
        a.stop();
        printf("stop timer over\n");
    }
    system("pause");
    return 0;
}