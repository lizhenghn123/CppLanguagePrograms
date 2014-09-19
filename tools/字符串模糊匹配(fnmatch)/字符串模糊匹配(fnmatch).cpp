#include <iostream>
#include <vector>
#include "OsDefine.h"
using namespace std;

/******************************************************
               C++实现字符串的模糊匹配
用于实现类似需求：
	准入授权配置文件有时候分了好几个维度进行配置，例如 company|product|sys这种格式的配置:
	1.配置 "sina|weibo|pusher" 表示 sina公司weibo产品pusher系统能够准入，而"sina|weibo|sign"不允许准入
	2.配置 "sina|*|pusher” 表示sina公司所有产品的pusher系统都能够准入
	3.配置 “*|*|pusher” 表示所有公司的所有产品的pusher系统都能够准入

以上需求及下面测试代码来源于网络：http://www.cnblogs.com/Creator/archive/2013/03/25/2981186.html；
其中fnmatch是Linux下特有函数，因此从Google-glog中摘取了部分跨平台实现，请看：https://code.google.com/p/google-glog
******************************************************/

#ifdef OS_WINDOWS
/* Bits set in the FLAGS argument to `fnmatch'. copy from fnmatch.h(linux) */
#define	FNM_PATHNAME	(1 << 0) /* No wildcard can ever match `/'.  */
#define	FNM_NOESCAPE	(1 << 1) /* Backslashes don't quote special chars.  */
#define	FNM_PERIOD		(1 << 2) /* Leading `.' is matched only explicitly.  */
#define	FNM_NOMATCH		1

#define fnmatch fnmatch_win

/**copy from Google-glog*/
bool SafeFNMatch(const char *pattern, size_t patt_len, const char *str, size_t str_len)
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
        if (pattern[p] == str[s]  ||  pattern[p] == '?')
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
            }
            while (s != str_len);
            return false;
        }
        return false;
    }
}

/**注意：Windows平台下尚未实现最后一个参数flags的功能！！！*/
int fnmatch_win(const char *pattern, const char *name, int flags = 0)
{
    if(SafeFNMatch(pattern, strlen(pattern), name, strlen(name)))
        return 0;
    else
        return FNM_NOMATCH;
}

#else
#include <fnmatch.h>
#endif

int main()
{
    const char *orgin_str = "sina|weibo|pusher";
    char pattern_arr[][20] =
    {
        {"sina|*|pusher"},
        {"sina|*|*"},
        {"*|weibo|*"},
        //不能被匹配的
        {"sina|pic|*"},
        {"*|*|sign"},
        {"*|weibo|sign"},
        {"*|pic|sign"},
        {"sina|pic|sign"},

        {"*|*|*"}
    };
    static int pattern_arr_size = sizeof(pattern_arr) / sizeof(pattern_arr[0]);
    vector<char *> vec_str;
    for(int i = 0; i < pattern_arr_size; i ++)
    {
        vec_str.push_back(pattern_arr[i]);
    }
    std::cout << "Origin Str: " << orgin_str << "\n\n";
    int ret;
    for(size_t i = 0; i < vec_str.size(); i++)
    {
        ret = fnmatch(vec_str.at(i), orgin_str, FNM_PATHNAME);
        if(ret == FNM_NOMATCH)
        {
            cout << "sorry, I'm failed: [" << vec_str.at(i) << "]\n";
        }
        else
        {
            cout << "OK, I'm success: [" << vec_str.at(i) << "]\n";
        }
    }
    return 0;
}