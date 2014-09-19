#include <windows.h>
#include "ByteArray.h"
#include <iostream>
#include <string>
using namespace std;

void test_bytebuffer()
{
    ByteArray byteArray(100);
    byteArray.WriteNumber(1037);
    byteArray.WriteBool(true);
    byteArray.WriteChars("char test");
    string str("string_test_zxcvbnm");
    byteArray.WriteString(str);
    byteArray.WriteNumber((float)2.45);
    byteArray.WriteNumber((double)1.79);

    int intVal = 0;
    byteArray.ReadNumber(&intVal);
    cout << intVal << "\n";

    bool bVal = false;
    bVal = byteArray.ReadBool();
    cout << bVal << "\n";

    char chars[100] = { 0 };
    byteArray.ReadChars(chars, 9);
    cout << chars << "\n";

    string strOut = byteArray.ReadString();
    cout << strOut << "\n";

    float f = 0.0;
    byteArray.ReadNumber(&f);

    cout << f << "\n";
    double d = byteArray.Read<double>();
    cout << d << "\n";
}

int main()
{
    test_bytebuffer();
    system("pause");
    return 0;
}