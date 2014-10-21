#include "base/DatetimeUtil.h"
#include "Define.h"
#include <time.h>
#include <stdlib.h>

namespace zl{
namespace base {

#ifdef OS_WINDOWS
#define ZL_LOCALTIME(a, b)  localtime_s(b, a)
#else
#define ZL_LOCALTIME(a, b)  localtime_r(a, b)
#endif

bool DateTimeUtil::IsLeapYear(int year)
{
    return ((0 == year % 4) && (year % 100 != 0)) || (0 == year % 400);
}

void DateTimeUtil::CurrentDateTime(struct tm *ptm)
{
    time_t now = time(NULL);
    ZL_LOCALTIME(&now, ptm);
}

void DateTimeUtil::CurrentDateTime(char *buf, size_t size)
{
    struct tm result;
    CurrentDateTime(&result);

    ZL_SNPRINTF(buf, size, "%04d-%02d-%02d %02d:%02d:%02d"
                , result.tm_year + 1900, result.tm_mon + 1, result.tm_mday
                , result.tm_hour, result.tm_min, result.tm_sec);
}

std::string DateTimeUtil::CurrentDateTime()
{
    char buf[sizeof("YYYY-MM-DD HH:SS:MM")];
    CurrentDateTime(buf, sizeof(buf));
    return buf;
}

void DateTimeUtil::CurrentDate(char *buffer, size_t size)
{
    struct tm result;
    time_t now = time(NULL);

    ZL_LOCALTIME(&now, &result);
    ZL_SNPRINTF(buffer, size, "%04d-%02d-%02d", result.tm_year + 1900, result.tm_mon + 1, result.tm_mday);
}

std::string DateTimeUtil::CurrentDate()
{
    char buffer[sizeof("YYYY-MM-DD")];
    CurrentDate(buffer, sizeof(buffer));
    return buffer;
}

void DateTimeUtil::CurrentTime(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm result;
    ZL_LOCALTIME(&now, &result);
    ZL_SNPRINTF(buffer, size, "%02d:%02d:%02d", result.tm_hour, result.tm_min, result.tm_sec);
}

std::string DateTimeUtil::CurrentTime()
{
    char time_buffer[sizeof("HH:SS:MM")];
    CurrentTime(time_buffer, sizeof(time_buffer));
    return time_buffer;
}

bool DateTimeUtil::StringToDataTime(const char *strTime, struct tm *datetime)
{
    const char *pos = strTime;

    size_t str_len = strlen(pos);
    if(str_len != sizeof("YYYY-MM-DD HH:MM:SS") - 1)
        return false;

    if((pos[4] != '-') || (pos[7] != '-') || (pos[10] != ' ') || (pos[13] != ':') || (pos[16] != ':'))
        return false;

    datetime->tm_year = atoi(pos);
    if((datetime->tm_year > 3000) || (datetime->tm_year < 1900))
        return false;

    pos += sizeof("YYYY");
    datetime->tm_mon = atoi(pos);
    if((datetime->tm_mon > 12) || (datetime->tm_mon < 1))
        return false;

    pos += sizeof("MM");
    datetime->tm_mday = atoi(pos);
    if(datetime->tm_mday < 1)
        return false;
    // 闰年二月29天
    if((DateTimeUtil::IsLeapYear(datetime->tm_year)) && (2 == datetime->tm_mon) && (datetime->tm_mday > 29))
        return false;
    else if(datetime->tm_mday > 28)
        return false;

    pos += sizeof("DD");
    datetime->tm_hour = atoi(pos);
    if((datetime->tm_hour > 24) || (datetime->tm_hour < 0))
        return false;

    pos += sizeof("HH");
    datetime->tm_min = atoi(pos);
    if((datetime->tm_min > 60) || (datetime->tm_min < 0))
        return false;

    pos += sizeof("MM");
    datetime->tm_sec = atoi(pos);
    if((datetime->tm_sec > 60) || (datetime->tm_sec < 0))
        return false;

    datetime->tm_isdst = 0;
    datetime->tm_wday = 0;
    datetime->tm_yday = 0;

    // 计算改天是一年中的第几天
    for(int i = 1; i <= datetime->tm_mon; ++i)
    {
        if(i == datetime->tm_mon)
        {
            // 刚好是这个月
            datetime->tm_yday += datetime->tm_mday;
        }
        else
        {
            // 1,3,5,7,8,10,12
            if((1 == i) || (3 == i) || (5 == i) || (7 == i) || (8 == i) || (10 == i) || (12 == i))
            {
                datetime->tm_yday += 31;
            }
            else if(2 == i)
            {
                if(DateTimeUtil::IsLeapYear(datetime->tm_year))
                    datetime->tm_yday += 29;
                else
                    datetime->tm_yday += 28;
            }
            else
            {
                datetime->tm_yday += 30;
            }
        }
    }

    // 月基数
    static int leap_month_base[] = { -1, 0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6 };
    static int common_month_base[] = { -1, 0, 3, 3, 6, 1, 4, 0, 3, 5, 0, 3, 5 };

    int year_base;
    int *month_base;
    if(DateTimeUtil::IsLeapYear(datetime->tm_year))
    {
        year_base = 2;
        month_base = leap_month_base;
    }
    else
    {
        year_base = 1;
        month_base = common_month_base;
    }

    // 计算星期几
    datetime->tm_wday = (datetime->tm_year
                         + datetime->tm_year / 4
                         + datetime->tm_year / 400
                         - datetime->tm_year / 100
                         - year_base
                         + month_base[datetime->tm_mon]
                         + datetime->tm_mday) / 7;

    // 年月处理
    datetime->tm_mon -= 1;
    datetime->tm_year -= 1900;

    return true;
}

bool DateTimeUtil::StringToDataTime(const char *strTime, time_t *datetime)
{
    struct tm result;
    if(!StringToDataTime(strTime, &result))
        return false;

    *datetime = mktime(&result);
    return true;
}

void DateTimeUtil::DateTimeToString(struct tm *datetime, char *buf, size_t size)
{
    ZL_SNPRINTF(buf, size, "%04d-%02d-%02d %02d:%02d:%02d"
                , datetime->tm_year + 1900, datetime->tm_mon + 1, datetime->tm_mday
                , datetime->tm_hour, datetime->tm_min, datetime->tm_sec);
}

std::string DateTimeUtil::DateTimeToString(struct tm *datetime)
{
    char buf[sizeof("YYYY-MM-DD HH:SS:MM")];
    DateTimeToString(datetime, buf, sizeof(buf));
    return buf;
}

void DateTimeUtil::DateToString(struct tm *datetime, char *buf, size_t size)
{
    ZL_SNPRINTF(buf, size, "%04d-%02d-%02d"
                , datetime->tm_year + 1900, datetime->tm_mon + 1, datetime->tm_mday);
}

std::string DateTimeUtil::DateToString(struct tm *datetime)
{
    char buf[sizeof("YYYY-MM-DD")];
    DateToString(datetime, buf, sizeof(buf));
    return buf;
}

void DateTimeUtil::TimeToString(struct tm *datetime, char *buf, size_t size)
{
    ZL_SNPRINTF(buf, size, "%02d:%02d:%02d", datetime->tm_hour, datetime->tm_min, datetime->tm_sec);
}

std::string DateTimeUtil::TimeToString(struct tm *datetime)
{
    char buf[sizeof("HH:SS:MM")];
    TimeToString(datetime, buf, sizeof(buf));
    return buf;
}

} // namespace base
} // namespace zl