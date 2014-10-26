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
        TimerQueue::instance()->Start();
        int i = 1, j=2, m=3, n=4;
        Timer a(1000,func,(void *)&i,Timer::TIMER_CIRCLE);
        Timer a1(2000,func,(void *)&j,Timer::TIMER_ONCE);
        Timer a2(3000,func,(void *)&m,Timer::TIMER_ONCE);
        Timer a3(1000,func,(void *)&n,Timer::TIMER_CIRCLE);
        Sleep(2000);
        a.Start();
        a1.Start();
        a2.Start();
        a3.Start();
        //a.Start();
        //a1.Start();
        //a2.Start();
        //a3.Start();

        a.Reset(2000);
        a1.Stop();
        a3.Stop();
        //while(1);
        Sleep(20000);
        a.Stop();
        printf("stop timer over\n");
    }
    system("pause");
    return 0;
}