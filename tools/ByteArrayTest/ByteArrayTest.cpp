#include <windows.h>
#include "base/ByteArray.h"
#include <iostream>
#include <string>
using namespace std;

void test_bytebuffer()
{
    ByteArray byteArray(1);
    byteArray.WriteNumber(1037);
    byteArray.WriteBool(true);
    byteArray.WriteChars("char test");
	cout << "already have size : " << byteArray.Size() <<" ; and have available : " << byteArray.Available() << "\n";
    string str("string_test_zxcvbnm");
    byteArray.WriteString(str);
    byteArray.WriteNumber((float)2.45);
	cout << "already have size : " << byteArray.Size() <<" ; and have available : " << byteArray.Available() << "\n";
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

	double d2 = byteArray.Read<double>();
	cout << d2 << "\n";
}

int main()
{
    test_bytebuffer();
    system("pause");
    return 0;
}