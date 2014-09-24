#include <iostream>
#include "base/StopWatch.h"
#include "base/HighPrecisionTime.h"

#ifdef OS_WINDOWS
#include <Windows.h>
#else
#include <system.h>
#endif

//睡眠，以毫秒为单位
void MySleep(long millseconds)
{
#ifdef OS_WINDOWS
    Sleep(millseconds);
#else
    usleep(millseconds * 1000);
#endif
}

int main()
{
    ZL::StopWatch watch;
    watch.Reset();
    MySleep(1000);
    std::cout << watch.ElapsedTime() << "\n";
    watch.Reset();
    MySleep(500);
    std::cout << watch.ElapsedTimeInMill() << "\n";
    watch.Reset();
    MySleep(800);
    std::cout << watch.ElapsedTimeInMicro() << "\n";
    std::cout << "-------------------------------------------\n";
    {
#ifdef OS_WINDOWS
        HighPrecisionTime watch;
        watch.Reset();
        Sleep(1000);
        std::cout << watch.ElapsedTime() << "\n";
        watch.Reset();
        Sleep(500);
        std::cout << watch.ElapsedTimeInMill() << "\n";
        watch.Reset();
        Sleep(800);
        std::cout << watch.ElapsedTimeInMicro() << "\n";
#endif
    }
    system("pause");
    return 0;
}