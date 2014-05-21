#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "osDefine.h"
#include "Demangle.h"

////Demangle.h Demangle.cpp 来源于glog,但该实现只可解析linux系统gcc编译器的名字重整。

#ifdef OS_WINDOWS
#include <Windows.h>
#include <DbgHelp.h>				/*用于实现将重整后的名字解析为原始名字*/
#pragma comment(lib,"DbgHelp.lib")
#elif defined(OS_LINUX)
#include<cxxabi.h>					/*用于实现将重整后的名字解析为原始名字*/
#else
#error "You must be include osDefine.h firstly"
#endif

int /*__cdecl*/ func(int);//windows平台下在函数名前可加__cdecl、__stdcall、__fastcall，默认__cdecl

float  func(float);

int    func(const std::vector<std::string>& vec);

namespace NP1
{
	int func(int);

	class C
	{
	public:
		int func(int);
	};
};

namespace NP2
{
	int func(int);

	class C
	{
	public:
		int func(int);
	};
};

#define IMPLEMENT_ALL   /**打开该宏，则定义以上函数实现*/
#ifdef IMPLEMENT_ALL

int func(int) { return 1; }

float func(float) { return (float)1.11; }

int func(const std::vector<std::string>& vec) { return 0; }

namespace NP1
{
	int func(int) { return 2; }

	int C::func(int) { return 3; }
};

namespace NP2
{
	int func(int) { return 4; }

	int C::func(int) { return 5; }
};

#endif

/******************************************************
根据重整后的名字解析出原函数原型名字（Windows/Linux）
*******************************************************/
void UnDecorateName(const char *szDecorateName)
{
	const size_t max_size = 1024;
	//char szDecorateName[max_size] = {0};
	char szUnDecorateName[max_size] = {0};
	//printf("Please Input Mangled Name: ");
	//scanf("%s", szDecorateName);

#ifdef OS_WINDOWS
	if (::UnDecorateSymbolName(szDecorateName, szUnDecorateName, sizeof(szUnDecorateName), UNDNAME_COMPLETE) == 0)
	{
		printf("UnDecorateSymbolName Failed. GetLastError() = %d", GetLastError());
	}
	else
	{
		printf("Name after  Mangled : %s \nName before Mangled : %s\n", szDecorateName, szUnDecorateName);
	}
#else
	int status;
	size_t n = max_size;
	abi::__cxa_demangle(szDecorateName,szUnDecorateName,&n,&status);
	printf("Name after  Mangled : %s \nName before Mangled : %s\n", szDecorateName, szUnDecorateName);
#endif
}

class H
{
public : 
	H();

};
int main(void)
{
	int i = 1;
	float f = 1.0;
	std::vector<std::string> vec;
	NP1::C *pac = new NP1::C;
	NP2::C *pbc = new NP2::C;

#if 1
	func(f);
	func(i);

	NP1::func(i);
	pac->func(i);

	NP2::func(i);
	pbc->func(i);

	func(vec);
#endif
	UnDecorateName("?func@@YAHABV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@@Z");
	std::cout << "---------------------------------\n";
	UnDecorateName("?func@NP1@@YAHH@Z");
	std::cout << "---------------------------------\n";
	UnDecorateName("??0H@@QAE@XZ");
	std::cout << "---------------------------------\n";

	//下面三个是linux下gcc编译器重整后的名字，在Windows下解析不成功
	UnDecorateName("_ZTCN10LogMessage9LogStreamE0_So");
	UnDecorateName("_ZN3FooD1Ev");
	UnDecorateName("_ZNSoD0Ev");
	
	//H h;
	char buf[4096];
	Demangle("_ZTCN10LogMessage9LogStreamE0_So",buf,4096);
	std::cout << buf <<"\n---------------------------------\n";

	memset(buf,'0',4096);
	Demangle("_ZN3FooD1Ev",buf,4096);
	std::cout << buf <<"\n---------------------------------\n";

	memset(buf,'0',4096);
	Demangle("??0H@@QAE@XZ",buf,4096);		//Windows,VS 2010重整的名字，glog的Demangle也是解析不成功
	std::cout << buf <<"\n---------------------------------\n";

	//system("pause");
	return 0;
}