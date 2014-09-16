#include <iostream>
#include <string>
#include "Base64.h"

void test()
{
	{
		char *src = "123456"; //"hello world";
		char dst[100] = {0};
		Base64Encode(src, strlen(src), dst);
		std::cout << src << "\t" <<dst << "\n";
	}
	{
		const char *src = "hello world";
		std::string dst;
		int size = Base64Encode(src, strlen(src), dst);
		std::cout << src << "\t" <<dst << "\n";
	}
	{
		std::string src = "my name is lz";
		std::string dst;
		int size = Base64Encode(src, dst);
		std::cout << src << "\t" <<dst << "\n";
	}

	{
		const char *src = "MTIzNDU2";
		char dst[100] = {0};
		Base64Decode(src, strlen(src), dst);
		std::cout << src << "\t" <<dst << "\n";
	}
	{
		const char *src = "aGVsbG8gd29ybGQ=";
		std::string dst;
		int size = Base64Decode(src, strlen(src), dst);
		std::cout << src << "\t" <<dst << "\n";
	}
	{
		std::string src = "bXkgbmFtZSBpcyBseg==";
		std::string dst;
		int size = Base64Decode(src, dst);
		std::cout << src << "\t" <<dst << "\n";
	}
}
int main()
{
	test();

	system("pause");
	return 0;
}