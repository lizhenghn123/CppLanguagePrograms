#include <iostream>
#include "base/Bitset.h"
using namespace std;
using namespace zl;

template< typename T>
void test_t()
{
    T i = 0;
    Bitset<T> p(i);
    p.set(9);
    cout << p << "\n";
    cout << p.test(9) << "\n";
    p.clr(9);
    cout << p << "\n";
    cout << p.test(9) << "\n";
    cout << p.test(10) << "\n";

    p.dump();
    p.set(9);
    p.set(10);
    p.dump();
    cout << p << "\n";
    p.clr(9);
    cout << p << "\n";
}

int main(int argc, char* argv[])
{
    cout << "*****************\n";
    test_t<int>();

    cout << "*****************\n";
    test_t<long>();

    system("pause");

    return 0;
}