#include <iostream>
#include "int128.h"
using namespace std;

int main()
{
    toft::UInt128 u1(INT_MAX);
    toft::UInt128 u2(INT_MAX);

    toft::UInt128 u3 = u1 + u2;

    cout << INT_MAX << "\n";
    cout << u1 << "\n";
    cout << u2 << "\n";
    cout << u3 << "\n";

    for(int i = 0; i< 100; ++i)
    {
        u3 += u3; 
        cout << u3 << "\n"; //error
    }

    system("pause");
}