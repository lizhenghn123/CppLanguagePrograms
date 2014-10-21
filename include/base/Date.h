// ***********************************************************************
// Filename         : Date.h
// Author           : LIZHENG
// Created          : 2014-09-17
// Description      : 日期类
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-09-17
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_DATE_H
#define ZL_DATE_H
#include <string>

namespace zl{
namespace base {
//#define IsLeapYear(y) ((!(y % 4)) ? (((!(y % 400)) && (y % 100)) ? 1 : 0) : 0)

class Date
{
public:
    Date();
    Date(int year, int month, int day);

public:
    static Date Today();
    static Date UtcToday();
    static bool IsLeapYear(int year);
    static bool IsValid(int year, int month, int day);
    static int  DaysInMonth(int year, int month);
    static int  DaysInPreviousMonth(int year, int month);
    static int  DaysInNextMonth(int year, int month);
    static int  Compare(const Date &lhs, const Date &rhs);
    static int  DaysDiff(const Date &from, const Date &to);

public:
    bool Set(int year, int month, int day);
    bool Year(int year);
    bool Month(int month);
    bool Day(int day);

    int  Year()
    {
        return year_;
    }
    int  Month()
    {
        return month_;
    }
    int  Day()
    {
        return day_;
    }
    bool IsLeapYear()
    {
        return IsLeapYear(year_);
    }

    Date &AddYears(int years);
    Date &AddMonths(int months);
    Date &AddDays(int days);

    Date NextDay() const;
    Date NextMonth() const;
    Date NextYear() const;

    bool IsEqual(const Date &rhs) const;

    int  DaysDiff(const Date &to) const; // 距离日期to有多少天
    int  DaysToNextYear() const;         // 距离下一年的今天有多少天
    int  DaysToNextMonth() const;        // 距离下个月的今天有多少天
    int  DaysToPreviousYear() const;     // 距离上一年的今天有多少天
    int  DaysToPreviousMonth() const;    // 距离上个月的今天有多少天

    std::string ToString() const;

public:
    Date &operator+=(int days);
    Date &operator-=(int days);
    Date &operator++();
    Date &operator--();
    Date  operator++(int);
    Date  operator--(int);
    Date  operator+(int days);
    Date  operator-(int days);

    bool operator<(const Date &rhs) const
    {
        return Date::Compare(*this, rhs) < 0;
    }

    bool operator>(const Date &rhs) const
    {
        return Date::Compare(*this, rhs) > 0;
    }

    bool operator==(const Date &rhs) const
    {
        return Date::Compare(*this, rhs) == 0;
    }

    bool operator<=(const Date &rhs) const
    {
        return Date::Compare(*this, rhs) <= 0;
    }

    bool operator>=(const Date &rhs) const
    {
        return Date::Compare(*this, rhs) >= 0;
    }

    bool operator!=(const Date &rhs) const
    {
        return Date::Compare(*this, rhs) != 0;
    }
    //	friend bool operator<(const Date& lhs, const Date& rhs) const;
private:
    void AdjustMonth();

private:
    int year_;    // [1900..]
    int month_;   // [1..12]
    int day_;     // [1..31]
};

} // namespace base
} // namespace zl
#endif  /* ZL_DATE_H */