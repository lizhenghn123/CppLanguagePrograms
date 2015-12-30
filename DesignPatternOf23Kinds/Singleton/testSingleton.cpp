#include <iostream>
#include "Singleton.h"
using namespace std;

int main()
{
    //----------------------------------------------------------------
    Singleton *sgn = Singleton::Instance();
    //----------------------------------------------------------------
    Test *test = Test_Ton::getSingleTonPtr();
    test->i = 100;
    test->print();
    Test_Ton::getSingleTonPtr()->print();
    //----------------------------------------------------------------
    FGHYU *gh = FGHYU::getSingleTonPtr();
    gh->i = 1453;
    gh->print();
    FGHYU::getSingleTonPtr()->print();

    system("pause");
    return 0;
}