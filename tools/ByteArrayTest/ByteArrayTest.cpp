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
    cout << "size : " << byteArray.Size() << " ; ReadableBytes : " << byteArray.ReadableBytes() << " ; WritableBytes : " << byteArray.WritableBytes()  << "\n";
    string str("string_test_zxcvbnm");
    byteArray.WriteString(str);
    byteArray.WriteNumber((float)2.45);

    byteArray.WriteNumber((double)1.79);
    cout << "size : " << byteArray.Size() << " ; ReadableBytes : " << byteArray.ReadableBytes() << " ; WritableBytes : " << byteArray.WritableBytes()  << "\n";

    int intVal = 0;
    byteArray.ReadNumber(&intVal);
    cout << intVal << "\n";
    cout << "size : " << byteArray.Size() << " ; ReadableBytes : " << byteArray.ReadableBytes() << " ; WritableBytes : " << byteArray.WritableBytes()  << "\n";
    bool bVal = false;
    bVal = byteArray.ReadBool();
    cout << bVal << "\n";

    byteArray.WriteNumber(123456);

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

    cout << byteArray.Read<int>() << "\n";

	double d2 = byteArray.Read<double>();
	cout << d2 << "\n";

    {
        ByteArray ba(1);
        ba.WriteChars(byteArray.Data(), byteArray.Size());
        cout << byteArray.Read<int>() << "\n";
    }
}

void test_bytebuffer2()
{
    cout << "---------------------------------\n";
     ByteArray ba1(1);
     ba1.WriteNumber<int>(12343);

     ByteArray ba2(1);
     //ba2.WriteChars(ba1.Data());    // error!!1
     ba2.WriteChars(ba1.Data(), ba1.Size());
     cout << ba2.Read<int>() << "\n";
}
int main()
{
    test_bytebuffer();
    test_bytebuffer2();
    system("pause");
    return 0;
}