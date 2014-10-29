#include <windows.h>
#include "base/ByteArray.h"
#include <iostream>
#include <string>
using namespace std;

void test_bytebuffer()
{
    ByteArray byteArray(1);
    byteArray.writeNumber(1037);
    byteArray.writeBool(true);
    byteArray.writeChars("char test");
    cout << "size : " << byteArray.size() << " ; readableBytes : " << byteArray.readableBytes() << " ; WritableBytes : " << byteArray.writableBytes()  << "\n";
    string str("string_test_zxcvbnm");
    byteArray.writeString(str);
    byteArray.writeNumber((float)2.45);

    byteArray.writeNumber((double)1.79);
    cout << "size : " << byteArray.size() << " ; readableBytes : " << byteArray.readableBytes() << " ; WritableBytes : " << byteArray.writableBytes()  << "\n";

    int intVal = 0;
    byteArray.readNumber(&intVal);
    cout << intVal << "\n";
    cout << "size : " << byteArray.size() << " ; readableBytes : " << byteArray.readableBytes() << " ; WritableBytes : " << byteArray.writableBytes()  << "\n";
    bool bVal = false;
    bVal = byteArray.readBool();
    cout << bVal << "\n";

    byteArray.writeNumber(123456);

    char chars[100] = { 0 };
    byteArray.readChars(chars, 9);
    cout << chars << "\n";

    string strOut = byteArray.readString();
    cout << strOut << "\n";
	
	
    float f = 0.0;
    byteArray.readNumber(&f);

    cout << f << "\n";
    double d = byteArray.read<double>();
    cout << d << "\n";

    cout << byteArray.read<int>() << "\n";

	double d2 = byteArray.read<double>();
	cout << d2 << "\n";

    {
        ByteArray ba(1);
        ba.writeChars(byteArray.data(), byteArray.size());
        cout << byteArray.read<int>() << "\n";
    }
}

void test_bytebuffer2()
{
    cout << "---------------------------------\n";
     ByteArray ba1(1);
     ba1.writeNumber<int>(12343);

     ByteArray ba2(1);
     //ba2.writeChars(ba1.Data());    // error!!1
     ba2.writeChars(ba1.data(), ba1.size());
     cout << ba2.read<int>() << "\n";
}
int main()
{
    test_bytebuffer();
    test_bytebuffer2();
    system("pause");
    return 0;
}