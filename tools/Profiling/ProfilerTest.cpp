#include <iostream>
#include <hash_map>
#include <map>
#include <Windows.h>
#include "Profiler.h"
using namespace std;

void print()
{
    PROFILER_PROFILING(__FUNCTION__);
    long long num = 1;
    for(int i = 1; i < 20; ++i)
    {
        //printf("print ing\n");
        num *= i;
    }
    printf("print success[%lld]\n", num);
    Sleep(50);
}

int main()
{
    //PROFILING_TIME("one");
    Profiler::ProfilerUnit entry1("one");
    {
        PROFILER_PROFILING("two");
        print();
    }
    Sleep(600);
    entry1.finish();

    {
        PROFILER_PROFILING("two");
        print();
        Sleep(100);
    }

    Profiler::getInstance().dump();
    system("pause");
}