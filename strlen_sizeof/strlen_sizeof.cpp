#include <iostream>

/*
sizeof()与strlen()的区别

1. sizeof是C++中的一个关键字，而strlen是C语言中的一个函数；
2. sizeof求的是字节长度，而strlen求的却是实际长度；
3. sizeof求的是系统分配的内存总量，而strlen通常遇到'\0'时即结束求值计算
3  sizeof可以用在任何类、结构体、对象、变量、类型等身上，strlen通常用于求字符串长度;
4. sizeof在计算一个类类型或类对象的的大小时，需要考虑其数据成员的内存布局（编译器可能会保持内存对齐）；
5. sizeof是在编译期计算的，而strlen是在运行时计算的,也因此sizeof可以用于类类型，这个特性使得sizeof在模板编程、元编程中大放异彩；
*/

class C;

//来自Boost源码，带安全检查指针删除
template<class T> inline void checked_delete(T *x)
{
	typedef char type_must_be_complete[ sizeof(T) ? 1 : -1]; // 如果T不是完整类型，则此句编译错误！
	(void) sizeof(type_must_be_complete);
	delete x;
}

class A
{
private:
	int    num;
	double score;
	char   ch;
};

class B
{
private:
	char   ch;
	int    num;
	double score;
};


int main()
{
	int i = 1;
	std::cout << sizeof(i) << "\n";   //sizeof(i) == sizeof(int) == 4
	
	char str1[] = "hello world";
	std::cout << sizeof(str1) << "\t" << strlen(str1) << "\n"; //12 11, 前者是因为为str1总共分配了12个字节大小内存（加上结束符'\0'）,而strlen计算时并不加上结束符

	char str2[100] = "hello world";
	std::cout << sizeof(str2) << "\t" << strlen(str2) << "\n"; //100 11, 前者是因为给str2显式分配了10个字节大小内存,而strlen计算的是实际长度（后面的没有使用）

	char *str3 = "hello world";
	std::cout << sizeof(str3) << "\t" << strlen(str3) << "\n"; //4 11, 前者是因为str是一个指针，通常一个指针大小是int的大小，即4字节

	A a;
	std::cout << sizeof(a) << "\t" << sizeof(A) << "\n"; //24 24, sizeof(a) == sizeof(A) == 8+8+8 == 24, 内存对齐

	A *b = new A;
	std::cout << sizeof(b) << "\t" << sizeof(*b) << "\n"; //4 24, sizeof(b) == sizeof(int) == 4, b是一个指针

	std::cout << sizeof(A) << "\t" << sizeof(B) << "\n"; //24 16，sizeof(A) == 8+8+8 == 24, sizeof(B) == 1+4+(3)+8 == 16,括号中的3表示为了对齐而剩余的3个字节

	C * c = NULL;
	//checked_delete(c);  //编译失败，因为C是不完整类型，只有声明，没有定义
	
	system("pause");
	return 0;
}

/***
输出：
4
12      11
100     11
4       11
24      24
4       24
24      16
***/