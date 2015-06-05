#include <iostream>
#include <vector>
#include <assert.h>
#include "utility/WildCard.h"
#include "OsDefine.h"
using namespace std;

void test_WildCard1()
{
    assert(WildCard::fnmatch("*.c", "test.c") == 0);
    assert(WildCard::fnmatch("*.c", "a.cpp") != 0);
    assert(WildCard::fnmatch("*.cpp", "this.cpp") == 0);
    assert(WildCard::fnmatch("*_test.cpp", "_test.cpp") == 0);
    assert(WildCard::fnmatch("lib*.a", "libc.a") == 0);
    assert(WildCard::fnmatch("lib?.so", "libm.so") == 0);
    assert(WildCard::fnmatch("lib?.so", "libpthread.so") != 0);
    assert(WildCard::fnmatch("lib*.so", "libpthread.so") == 0);
    assert(WildCard::fnmatch("lib*.so*", "libpthread.so.0.0.1") == 0);

    assert(WildCard::fnmatch("*.c", "a.c", FNM_FILE_NAME) == 0);
    assert(WildCard::fnmatch("*.c", "dir/a.c") == 0);
    assert(WildCard::fnmatch("*.c", "dir/a.c") == 0);

    assert(WildCard::fnmatch("*.c", "a.c", FNM_NOESCAPE) == 0);
    assert(WildCard::fnmatch("*.c", "a.C", FNM_NOESCAPE) != 0);
    assert(WildCard::fnmatch("*.c", "a.d", FNM_NOESCAPE) != 0);
}

/******************************************************
用于实现类似需求：
准入授权配置文件有时候分了好几个维度进行配置，例如 company|product|sys这种格式的配置:
1.配置 "sina|weibo|pusher" 表示 sina公司weibo产品pusher系统能够准入，而"sina|weibo|sign"不允许准入
2.配置 "sina|*|pusher” 表示sina公司所有产品的pusher系统都能够准入
3.配置 “*|*|pusher” 表示所有公司的所有产品的pusher系统都能够准入

以上需求及下面测试代码来源于网络：http://www.cnblogs.com/Creator/archive/2013/03/25/2981186.html；
其中fnmatch是Linux下特有函数，因此从Google-glog中摘取了部分跨平台实现，请看：https://code.google.com/p/google-glog
******************************************************/
void test_WildCard2()
{
    const char *orgin_str = "sina|weibo|pusher";
    char pattern_arr[][20] =
    {
        { "sina|*|pusher" },
        { "sina|*|*" },
        { "*|weibo|*" },
        //不能被匹配的
        { "sina|pic|*" },
        { "*|*|sign" },
        { "*|weibo|sign" },
        { "*|pic|sign" },
        { "sina|pic|sign" },

        { "*|*|*" }
    };
    static int pattern_arr_size = sizeof(pattern_arr) / sizeof(pattern_arr[0]);
    vector<char *> vec_str;
    for (int i = 0; i < pattern_arr_size; i++)
    {
        vec_str.push_back(pattern_arr[i]);
    }
    std::cout << "Origin Str: " << orgin_str << "\n\n";
    int ret;
    for (size_t i = 0; i < vec_str.size(); i++)
    {
        ret = WildCard::fnmatch(vec_str.at(i), orgin_str, FNM_PATHNAME);
        if (ret == FNM_NOMATCH)
        {
            cout << "sorry, I'm failed: [" << vec_str.at(i) << "]\n";
        }
        else
        {
            cout << "OK, I'm success: [" << vec_str.at(i) << "]\n";
        }
    }
}

int main()
{
    test_WildCard1();
    cout << "-----------------------------------\n";

    test_WildCard2();

    system("pause");
}