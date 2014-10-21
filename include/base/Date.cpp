#include "base/Date.h"
#include "Define.h"
#include <time.h>

namespace zl{
namespace base {

static int const gDaysInMonth[] =
{
    // 1   2   3   4   5   6   7   8   9   10  11  12
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

Date::Date()
{
    Set(1970, 1, 1);
}

Date::Date(int year, int month, int day)
{
    Set(year, month, day);
}

bool Date::Set(int year, int month, int day)
{
    if (!IsValid(year, month, day))
        return false;
    year_ = year;
    month_ = month;
    day_ = day;
    return true;
}

bool Date::Year(int year)
{
    year_ = year;
    AdjustMonth();
    return true;
}

bool Date::Month(int month)
{
    if (month <= 0 || month_ > 12)
        return false;
    month_ = month;
    AdjustMonth();
    return true;
}

bool Date::Day(int day)
{
    if (day <= 0 || day > 31)
        return false;
    if (day > DaysInMonth(year_, month_))
        return false;
    day_ = day;
    return true;
}

Date &Date::AddYears(int years)
{
    Year(year_ + years);
    return *this;
}

Date &Date::AddMonths(int months)
{
    if (months == 0)
        return *this;

    if (months > 0)
    {
        int years = months / 12;
        months %= 12;
        year_ += years;
        month_ += months;
        if (month_ > 12)
        {
            ++year_;
            month_ -= 12;
        }
    }
    else
    {
        months = -months;
        int years = months / 12;
        months %= 12;
        year_ -= years;
        month_ -= months;
        if (month_ < 1)
        {
            --year_;
            month_ += 12;
        }
    }

    AdjustMonth();
    return *this;
}

Date &Date::AddDays(int days)
{
    if (days == 0)
        return *this;

    if (days > 0)
    {
        for (;;) // Add years one by one.
        {
            int days_to_next_year = DaysToNextYear();
            if (days < days_to_next_year)
                break;
            AddYears(1);
            days -= days_to_next_year;
        }
        for (;;) // Add months one by one.
        {
            int days_to_next_month = DaysToNextMonth();
            if (days < days_to_next_month)
                break;
            AddMonths(1);
            days -= days_to_next_month;
        }
        day_ = day_ + days;
        int days_in_month = DaysInMonth(year_, month_);
        if (day_ > days_in_month) // Month overflow, adjust to next month.
        {
            day_ -= days_in_month;
            AddMonths(1);
        }
    }
    else
    {
        days = -days;
        for (;;)
        {
            int days_to_previous_year = DaysToPreviousYear();
            if (days < days_to_previous_year)
                break;
            AddYears(-1);
            days -= days_to_previous_year;
        }
        for (;;)
        {
            int days_to_previous_month = DaysToPreviousMonth();
            if (days < days_to_previous_month)
                break;
            AddMonths(-1);
            days -= days_to_previous_month;
        }
        day_ = day_ - days;
        if (day_ < 1)
        {
            int days_in_prev_month = DaysInPreviousMonth(year_, month_);
            day_ += days_in_prev_month;
            AddMonths(-1);
        }
    }

    return *this;
}

void Date::AdjustMonth()
{
    if (month_ == 2 && day_ > 28)
    {
        int days = DaysInMonth(year_, month_);
        if (day_ > days)
            day_ = days;
    }
}

int Date::DaysInPreviousMonth(int year, int month)
{
    return month > 1 ? DaysInMonth(year, month - 1) : DaysInMonth(year - 1, 12);
}

int Date::DaysInNextMonth(int year, int month)
{
    return month < 12 ? DaysInMonth(year, month + 1) : DaysInMonth(year + 1, 1);
}

Date Date::NextMonth() const
{
    Date result(*this);
    return result.AddMonths(1);
}

Date Date::NextYear() const
{
    Date result(*this);
    return result.AddYears(1);
}

Date &Date::operator+=(int days)
{
    AddDays(days);
    return *this;
}

Date &Date::operator-=(int days)
{
    AddDays(-days);
    return *this;
}

Date &Date::operator++()
{
    ++day_;
    if (day_ > DaysInMonth(year_, month_))
    {
        day_ = 1;
        ++month_;
        if (month_ > 12)
        {
            ++year_;
            month_ = 1;
        }
    }
    return *this;
}

Date &Date::operator--()
{
    --day_;
    if (day_ == 0)
    {
        --month_;
        if (month_ == 0)
        {
            month_ = 12;
            --year_;
        }
        day_ = DaysInMonth(year_, month_);
    }
    return *this;
}

Date Date::operator++(int)
{
    Date org(*this);
    ++*this;
    return org;
}

Date Date::operator--(int)
{
    Date org(*this);
    --*this;
    return org;
}

Date Date::operator+(int days)
{
    Date newd(*this);
    newd += days;
    return newd;
}
Date Date::operator-(int days)
{
    Date newd(*this);
    newd -= days;
    return newd;
}

bool  Date::IsEqual(const Date &rhs) const
{
    return year_ == rhs.year_ && month_ == rhs.month_ && day_ == rhs.day_;
}

int Date::Compare(const Date &lhs, const Date &rhs)
{
    if (lhs.year_ > rhs.year_)
        return 1;
    if (lhs.year_ < rhs.year_)
        return -1;
    if (lhs.month_ > rhs.month_)
        return 1;
    if (lhs.month_ < rhs.month_)
        return -1;
    if (lhs.day_ > rhs.day_)
        return 1;
    if (lhs.day_ < rhs.day_)
        return -1;
    return 0;
}

int  Date::DaysDiff(const Date &from, const Date &to)
{
    return from.DaysDiff(to);
}

int Date::DaysDiff(const Date &to) const
{
    if (to < *this)
        return to.DaysDiff(*this);
    int days = 0;
    Date from(*this);
    for (;;)
    {
        Date next_year = from.NextYear();
        if (next_year > to)
            break;
        days += from.DaysToNextYear();
        from = next_year;
    }
    for (;;)
    {
        Date next_month = from.NextMonth();
        if (next_month > to)
            break;
        days += from.DaysToNextMonth();
        from = next_month;
    }
    while (from < to)
    {
        ++from;
        ++days;
    }
    return days;
}

int Date::DaysToNextYear() const
{
    if (month_ > 2 || (month_ == 2 && day_ > 28))
    {
        return 365 + IsLeapYear(year_ + 1);
    }
    else
    {
        return 365 + IsLeapYear(year_);
    }
}

int Date::DaysToNextMonth() const
{
    int this_month_days = DaysInMonth(year_, month_);
    int next_month_days = DaysInNextMonth(year_, month_);
    if (day_ <= next_month_days)
        return this_month_days;
    return next_month_days + this_month_days - day_;
}

int Date::DaysToPreviousYear() const
{
    if (month_ < 2 || (month_ == 2 && day_ < 28))
    {
        return 365 + IsLeapYear(year_ - 1);
    }
    else
    {
        return 365 + IsLeapYear(year_);
    }
}

int Date::DaysToPreviousMonth() const
{
    int prev_month_days = DaysInPreviousMonth(year_, month_);
    return std::max(static_cast<int>(day_), prev_month_days);
}

std::string Date::ToString() const
{
    char buf[16];
    ZL_SNPRINTF(buf, 16, "%04d-%02d-%02d", year_, month_, day_);
    return buf;
}

bool Date::IsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

bool Date::IsValid(int year, int month, int day)
{
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > DaysInMonth(year, month))
        return false;
    return true;
}

int Date::DaysInMonth(int year, int month)
{
    int days = gDaysInMonth[month];
    if (month == 2 && IsLeapYear(year))
        ++days;
    return days;
}

Date Date::Today()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t); // NOLINT(runtime/threadsafe_fn)
    return Date(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}

Date Date::UtcToday()
{
    time_t t = time(NULL);
    struct tm *tm = gmtime(&t); // NOLINT(runtime/threadsafe_fn)
    return Date(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}

} // namespace base
} // namespace zl