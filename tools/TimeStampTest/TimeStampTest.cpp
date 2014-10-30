#include <iostream>
#include <time.h>
#include "base/Timestamp.h"
using namespace std;
using namespace zl::base;

int main()
{
    Timestamp time1 = Timestamp::now();
    time_t time2 = time(NULL);
    {
        const long long kMicroSecondsPerSecond = 1000 * 1000;
        char buf[32] = {0};
        time_t seconds = static_cast<time_t>(time1.microSeconds() / kMicroSecondsPerSecond);
        struct tm *tm_time = localtime(&seconds);
         printf("++++%ld\n", seconds);
        if (1)
        {
            int microseconds = static_cast<int>(time1.microSeconds() % kMicroSecondsPerSecond);
            _snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d:%06d\n",
                tm_time->tm_year + 1900, tm_time->tm_mon + 1, tm_time->tm_mday,
                tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec,
                microseconds);
            printf("%s\n", buf);
        }

        printf("%s\n", time1.toString().c_str());
        printf("%s\n", time1.toString(false).c_str());
    }
    {
        char buf[32] = {0};
        struct tm *tm_time = gmtime(&time2);
        printf("++++%ld\n", time2);
        if (1)
        {
            int microseconds = 000000;
            _snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d:%06d\n",
                tm_time->tm_year + 1900, tm_time->tm_mon + 1, tm_time->tm_mday,
                tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec,
                microseconds);
            printf("%s\n", buf);
        }
    }
    {
        Timestamp now = Timestamp::now();
        printf("%s\n", now.toString().c_str());
        Timestamp n3 = now + 3;
        printf("%s\n", n3.toString().c_str());
        n3 = 10.01 + n3;
        printf("%s\n", n3.toString().c_str());

        printf("diff : %f\n", Timestamp::timediff(n3, now));
    }
    system("pause");
}