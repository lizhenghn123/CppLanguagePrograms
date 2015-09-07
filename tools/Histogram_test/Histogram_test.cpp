#include <iostream>
#include "base/Histogram.h"
using namespace std;

int main()
{
    {
        zl::base::Histogram hi;
        hi.add(2.4);
        hi.add(4.3);
        hi.add(5.3);
        hi.add(4.6);
        hi.add(4.9);
        cout << hi.toString() << "\n";
    }
    {
        zl::base::Histogram hi;
        hi.add(24);
        hi.add(43);
        hi.add(53);
        hi.add(46);
        hi.add(49);
        cout << hi.toString() << "\n";
    }
    system("pause");
}