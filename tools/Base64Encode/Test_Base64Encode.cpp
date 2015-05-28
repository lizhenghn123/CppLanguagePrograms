#include <iostream>
#include <string>
#include "utility/Base64.h"
using namespace zl::base;
using namespace zl::util;
void test()
{
    {
        char *src = "123456"; //"hello world";
        char dst[100] = {0};
        base64Encode(src, strlen(src), dst);
        std::cout << src << "\t" << dst << "\n";
    }
    {
        const char *src = "hello world";
        std::string dst;
        int size = base64Encode(src, strlen(src), dst);
        std::cout << src << "\t" << dst << "\n";
    }
    {
        std::string src = "my name is lz";
        std::string dst;
        int size = base64Encode(src, dst);
        std::cout << src << "\t" << dst << "\n";
    }
    {
        const char *src = "MTIzNDU2";
        char dst[100] = {0};
        base64Decode(src, strlen(src), dst);
        std::cout << src << "\t" << dst << "\n";
    }
    {
        const char *src = "aGVsbG8gd29ybGQ=";
        std::string dst;
        int size = base64Decode(src, strlen(src), dst);
        std::cout << src << "\t" << dst << "\n";
    }
    {
        std::string src = "bXkgbmFtZSBpcyBseg==";
        std::string dst;
        int size = base64Decode(src, dst);
        std::cout << src << "\t" << dst << "\n";
    }
}
int main()
{
    test();
    system("pause");
    return 0;
}