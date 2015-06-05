// ***********************************************************************
// Filename         : WildCard.h
// Author           : LIZHENG
// Created          : 2015-06-05
// Description      : 通配符简单实现，注意：Windows平台下尚未实现最后一个参数flags的功能！
//
// Last Modified By : LIZHENG
// Last Modified On : 2015-06-05
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_WILDCARD_H
#define ZL_WILDCARD_H
#include <string>

#define OS_WINDOWS
//#define OS_LINUX

#ifdef OS_WINDOWS
/* Bits set in the FLAGS argument to `fnmatch'. copy from fnmatch.h(linux) */
#define FNM_PATHNAME    (1 << 0)        /* No WildCard can ever match `/'.  */
#define FNM_NOESCAPE    (1 << 1)        /* Backslashes don't quote special chars.  */
#define FNM_PERIOD      (1 << 2)        /* Leading `.' is matched only explicitly.  */
#define FNM_FILE_NAME    FNM_PATHNAME   /* Preferred GNU name.  */
#define FNM_LEADING_DIR (1 << 3)        /* Ignore `/...' after a match.  */
#define FNM_CASEFOLD    (1 << 4)        /* Compare without regard to case.  */
#define FNM_EXTMATCH    (1 << 5)        /* Use ksh-like extended matching. */

/* Value returned by `fnmatch' if STRING does not match PATTERN.  */
#define FNM_NOMATCH      1
#endif

/// WildCard 通配符
class WildCard
{
public:
    /// return 0 if str matches pattern, FNM_NOMATCH if there is no match or another non-zero value if there is an error.
    static int fnmatch(const char *pattern, const char *str, int flags = 0);

    /// return 0 if str matches pattern, FNM_NOMATCH if there is no match or another non-zero value if there is an error.
    static int fnmatch(const std::string& pattern, const std::string& str, int flags = 0);
};

#endif  /* ZL_WILDCARD_H */
