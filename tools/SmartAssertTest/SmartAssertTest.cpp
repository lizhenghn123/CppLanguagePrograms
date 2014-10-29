#include <iostream>
#include "base/SmartAssert.h"
using namespace std;
using namespace zl::base;

int main()
{
    int i = 2;
    ZL_ASSERT(i != 2);
    printf("---------------------------------\n");
    ZL_ASSERTEX(i != 2, "sdfs", 342, "zxcvbnm");
    printf("---------------------------------\n");
    //ZL_STATIC_ASSERT(sizeof(i)>=8);
    SMART_ASSERT(i != 2)(i);
    printf("---------------------------------\n");
    std::vector<int> vec;
    vec.push_back(5);
    SMART_ASSERT(vec.size() >= i)(i)(vec.size());
    printf("---------------------------------\n");
    SMART_ASSERT_LOG(vec.size() >= i)(i)(vec.size());
    printf("---------------------------------\n");
    system("pause");
    return 0;
}