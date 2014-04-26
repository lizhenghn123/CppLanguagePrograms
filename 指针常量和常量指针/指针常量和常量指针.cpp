#include <iostream>
#include <cstdlib>
using namespace std;

size_t strlen1(const char* s)
{
	return strlen(s);
}

void test_ptr()
{
	int a = 100, b = 200;

	//-------常量指针-------
	const int *p1 = &a;
	a = 300;     //OK,仍然可以通过原来的声明修改值，
	//*p1 = 56;  //Error,*p1是const int的，不可修改，即常量指针不可修改其指向地址
	p1 = &b;     //OK,指针还可以指向别处，因为指针只是个变量，可以随意指向；

	//-------指针常量-------//
	int*  const p2 = &a;
	a = 500;	 //OK,仍然可以通过原来的声明修改值，
	*p2 = 400;   //OK,指针是常量，指向的地址不可以变化,但是指向的地址所对应的内容可以变化
	//p2 = &b;	 //Error,因为p2是const 指针，因此不能改变p2指向的内容

	//-------指向常量的常量指针-------//
	const int* const p3 = &a;
	//*p3 = 1;	//Error
	//p3 = &b;	//Error
	a = 5000;	//OK,仍然可以通过原来的声明修改值

	//-------空指针-------//
	int *p4 = NULL;
	//printf("%d",*p4); //运行Error，使用指针时必须先判断是否空指针

	//-------野指针（悬浮、迷途指针）-------//
	int *p5 = new int(5);
	delete p5;
	//p5 = NULL; //一定要有这一步
	//printf("%d",*p5);  //隐藏bug，delete掉指针后一定要置0，不然指针指向位置不可控，运行中可导致系统挂掉

	//-------指针的内存泄漏-------//
	int *p6 = new int(6);
	p6 = new int(7); //p6原本指向的那块内存尚未释放，结果p6又指向了别处，原来new的内存无法访问，也无法delete了，造成memory leak
}

void process_value(/*const*/ int& i) {
	std::cout << "LValue processed: " << i << std::endl;
}

void process_value(int&& i) {
	std::cout << "RValue processed: " << i << std::endl;
}

int main()
{
	int a = 0;
	process_value(a);
	process_value(1);
	test_ptr();
	system("pause");
	return 0;
}