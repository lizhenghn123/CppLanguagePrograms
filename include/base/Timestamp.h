// ***********************************************************************
// Filename         : Timestamp.h
// Author           : LIZHENG
// Created          : 2014-10-21
// Description      :
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-10-21
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_TIMESTAMP_H
#define ZL_TIMESTAMP_H
#include <string>

#define ZL_MSEC_PER_SEC   (1000)
#define ZL_USEC_PER_SEC   (1000 * 1000)
#define ZL_TIME_SEC(time) ((time) / ZL_USEC_PER_SEC)

namespace zl{
namespace base {

typedef long long zl_time_t;

class Timestamp
{
public:
    Timestamp();
    explicit Timestamp(zl_time_t ms);
public:
    static Timestamp Now();
public:
    zl_time_t MicoSeconds() const
    {
        return microSeconds_;
    }
    zl_time_t MillSeconds() const
    {
        return microSeconds_ / ZL_MSEC_PER_SEC;
    }
    zl_time_t Seconds()     const
    {
        return microSeconds_ / ZL_USEC_PER_SEC;
    }

    struct tm  *GetTm(bool showlocaltime = true);
    std::string ToString(bool showlocaltime = true);

private:
    zl_time_t  microSeconds_;
};

//bool operator<(const Timestamp& lhs, const Timestamp& rhs)
//{
//    return lhs.MicoSeconds() < rhs.MicoSeconds();
//}

//bool operator==(const Timestamp& lhs, const Timestamp& rhs)
//{
//    return lhs.MicoSeconds() == rhs.MicoSeconds();
//}

} // namespace base
} // namespace zl
#endif  /* ZL_TIMESTAMP_H */