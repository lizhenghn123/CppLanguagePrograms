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
    zl::StopWatch watch;
    watch.reset();
    MySleep(1000);
    std::cout << watch.elapsedTime() << "\n";
    watch.reset();
    MySleep(500);
    std::cout << watch.elapsedTimeInMill() << "\n";
    watch.reset();
    MySleep(800);
    std::cout << watch.elapsedTimeInMicro() << "\n";
    std::cout << "-------------------------------------------\n";
    {
#ifdef OS_WINDOWS
        HighPrecisionTime watch;
        watch.reset();
        Sleep(1000);
        std::cout << watch.elapsedTime() << "\n";
        watch.reset();
        Sleep(500);
        std::cout << watch.elapsedTimeInMill() << "\n";
        watch.reset();
        Sleep(800);
        std::cout << watch.elapsedTimeInMicro() << "\n";
#endif
    }
    system("pause");
    return 0;
}