#include <iostream>

#if defined(_WIN32) || defined(WIN32)        /**Windows*/
#define WINDOWS_IMPL
#include <windows.h>
#include <time.h>           //time() 、 clock()
#include <Mmsystem.h>       //timeGetTime()
#pragma comment(lib, "Winmm.lib") //timeGetTime()
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(BSD)    /**Linux*/
#define LINUX_IMPL
#include <sys/time.h>        //gettimeofday()
#endif
#include <stdio.h>

/***********************************************************
通用的：
time_t time(time_t *tloc);     //返回从1970年1月1日0点以来的秒数,精度为秒
clock_t clock(): 返回该程序从启动到函数调用占用CPU的时间，精度为毫秒,但一般最小精度是33ms

Windows特有：
GetTickCount(): 返回从操作系统启动到现在所经过的毫秒数，精度毫秒，但最小精度是18ms
                返回值以32位的双字类型DWORD存储，因此可以存储的最大值是2^32 ms约为49.71天，
timeGetTime()：    返回以毫秒计的系统时间，该时间为从系统开启算起所经过的时间，精度为毫秒
QueryPerformanceCounter()： 返回高精确度性能计数器的值,精度为微妙,但是确切的精确计时的最小单位是与系统有关的

Linux特有：
gettimeofday(struct timeval *tv,struct timezone *tz); 获得当前精确时间（1970年1月1日到现在的时间）,精度为微秒
***********************************************************/

void MySleep(int sec_time)
{
    #if defined(WINDOWS_IMPL)
        Sleep(sec_time*1000);
    #elif defined(LINUX_IMPL)
        sleep(sec_time);
    #endif
}

void test_time()
{
    //通用的
    //用time()来计时  秒
    time_t timeBegin, timeEnd;
    timeBegin = time(NULL);
    MySleep(1);
    timeEnd = time(NULL);
    printf("%d\n", timeEnd - timeBegin);

    /*
     * Structure used in select() call, taken from the BSD file sys/time.h.
     */
    //struct timeval {
    //        long    tv_sec;         /* seconds */
    //        long    tv_usec;        /* and microseconds */
    //};
    timeval  val;

    //用clock()来计时  毫秒
    clock_t  clockBegin, clockEnd;
    clockBegin = clock();
    MySleep(1);
    clockEnd = clock();
    printf("%d\n", clockEnd - clockBegin);

#ifdef WINDOWS_IMPL
    //Windows

    //用GetTickCount()来计时  毫秒
    DWORD  dwGTCBegin, dwGTCEnd;
    dwGTCBegin = GetTickCount();
    Sleep(1000);
    dwGTCEnd = GetTickCount();
    printf("%d\n", dwGTCEnd - dwGTCBegin);

    //用timeGetTime()来计时  毫秒
    DWORD  dwBegin, dwEnd;
    dwBegin = timeGetTime();
    Sleep(1000);
    dwEnd = timeGetTime();
    printf("%d\n", dwEnd - dwBegin);
    
    //用QueryPerformanceCounter()来计时  微秒
    LARGE_INTEGER  large_interger;
    double dff;
    __int64  c1, c2;
    QueryPerformanceFrequency(&large_interger);
    dff = large_interger.QuadPart;
    QueryPerformanceCounter(&large_interger);
    c1 = large_interger.QuadPart;
    Sleep(1000);
    QueryPerformanceCounter(&large_interger);
    c2 = large_interger.QuadPart;
    printf("高精度计时器频率%lf\n", dff);
    printf("第一次计时器值%I64d 第二次计时器值%I64d 计时器差%I64d\n", c1, c2, c2 - c1);
    printf("计时%lf毫秒\n", (c2 - c1) * 1000 / dff);

#elif  defined(LINUX_IMPL)
    //Linux

    struct timeval tpstart,tpend;
    double timeuse;
    gettimeofday(&tpstart,NULL);
    sleep(1);
    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;//注意，秒的读数和微秒的读数都应计算在内
    printf("used time:%fus\n",timeuse);
#endif

}

int main()
{
    test_time();
	getchar();
    return 0;
}