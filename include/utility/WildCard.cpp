#include "WildCard.h"
#ifdef OS_LINUX
#include <fnmatch.h>
#endif

/**copy from Google-glog library*/
static bool SafeFNMatch(const char *pattern, size_t patt_len, const char *str, size_t str_len)
{
    size_t p = 0;
    size_t s = 0;
    while (1)
    {
        if (p == patt_len  &&  s == str_len)
            return true;
        if (p == patt_len)
            return false;
        if (s == str_len)
            return p + 1 == patt_len  &&  pattern[p] == '*';
        if (pattern[p] == str[s] || pattern[p] == '?')
        {
            p += 1;
            s += 1;
            continue;
        }
        if (pattern[p] == '*')
        {
            if (p + 1 == patt_len) return true;
            do
            {
                if (SafeFNMatch(pattern + (p + 1), patt_len - (p + 1), str + s, str_len - s))
                {
                    return true;
                }
                s += 1;
            } while (s != str_len);
            return false;
        }
        return false;
    }
}

/**注意：Windows平台下尚未实现最后一个参数flags的功能！！！*/
int WildCard::fnmatch(const char *pattern, const char *str, int flags/* = 0*/)
{
#ifdef OS_LINUX
    return ::fnmatch(pattern, str, flags);
#else
    if (SafeFNMatch(pattern, strlen(pattern), str, strlen(str)))
        return 0;
    else
        return FNM_NOMATCH;
#endif
}

int WildCard::fnmatch(const std::string& pattern, const std::string& str, int flags/* = 0*/)
{
    return WildCard::fnmatch(pattern.c_str(), str.c_str(), flags);
}
