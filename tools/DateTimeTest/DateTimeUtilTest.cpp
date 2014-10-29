#include <iostream>
#include "base/Date.h"
#include "base/DateTimeUtil.h"
using namespace std;
using namespace zl::base;

void testDate()
{
    Date d(2014, 9, 17);
    cout << d.toString() << "\n";
    Date d2 = d + 3;
    cout << d2.daysDiff(d) << "\n";
    Date d3 = d + 245;
    cout << Date::daysDiff(d, d3) << "\n";
    cout << (d < d3) << "\n";
}

int main()
{
    {
        const char *str = "1998-12-23 12:34:56";
        const char *pos = str;
        int i = atoi(pos);
        cout << i << "\n";
        pos = pos + sizeof("YYYY");
        cout << atoi(pos) << "\n";
        pos = pos + sizeof("DD");
        cout << atoi(pos) << "\n";
        pos = pos + sizeof("HH");
        cout << atoi(pos) << "\n";
        pos = pos + sizeof("MM");
        cout << atoi(pos) << "\n";
        pos = pos + sizeof("SS");
        cout << atoi(pos) << "\n";
        cout << "=====" << sizeof("YYYY") << "\n";
        const char *sss = "YYYY";
        cout << sizeof(sss) << "\t" << sizeof(*sss) << "\n";
    }
    {
        cout << DateTimeUtil::currentDateTime() << "\n";
        cout << DateTimeUtil::currentDate() << "\n";
        cout << DateTimeUtil::currentTime() << "\n";
    }
    cout << "\n--------------------------\n";
    testDate();
    system("pause");
    return 0;
}