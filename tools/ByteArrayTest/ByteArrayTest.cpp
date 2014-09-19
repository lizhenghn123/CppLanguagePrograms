#include <windows.h>
#include "ByteArray.h"
#include <iostream>
#include <string>
using namespace std;

void test_bytebuffer()
{
	ByteArray byteArray(100);
	//byteArray.setEndian(LIT_ENDIAN, LIT_ENDIAN);

	byteArray.WriteNumber(1037);
	byteArray.WriteBool(true);
	byteArray.WriteChars("char test");
	string str("string_test_zxcvbnm");
	byteArray.WriteString(&str);

	int intVal=0;
	byteArray.ReadNumber(&intVal);
	cout << intVal << endl;

	bool bVal=false;
	byteArray.ReadBool(&bVal);
	cout << bVal << endl;

	char chars[100] = { 0 };
	byteArray.ReadChars(chars, 9);
	cout << chars << endl;

	string strOut = byteArray.ReadString();
	cout << strOut << endl;
}

int main()
{
	test_bytebuffer();

    system("pause");
    return 0;
}