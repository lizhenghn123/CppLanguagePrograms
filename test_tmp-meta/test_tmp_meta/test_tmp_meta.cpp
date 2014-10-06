#include <iostream>
#include <vector>
#include "meta/meta_util.hpp"
using namespace std;

void test_type_pod()
{
	class s1
	{ };
	class s2
	{
	public:
		virtual ~s2(){ }
	};

	must_be_pod<int>      m1; // Ok, int   是 pod
	must_be_pod<void*>    m2; // Ok, void* 是 pod
	must_be_pod<s1>       m3; // Ok, s1    是 pod
	//	must_be_pod<s2>       m4; // Error, s2  不是 pod（s2 有虚函数）
	must_be_pod<void>     m5; // Ok, void 不是pod，但被特化了
}

void test_have_inherit()
{
	class Base
	{

	};
	class Derived : public Base
	{

	};
	class BaseImpl : private Base
	{

	};

	must_have_inherit<Derived, Base>     m1;  // Ok, Derived 是 Base 的子类
	//	must_have_inherit<Base, Derived>     m2;  // Error
	must_have_inherit<Derived, Derived>  m3;  // Ok
	must_have_inherit<Base, Base>        m4;  // Ok
	//	must_have_inherit<BaseImpl, Base>    m5;  // Error
}

void test_be_subscriptable()
{
	struct subs
	{
	public:
		int operator [](size_t index) const{ return 1; }
	};

	struct not_subs
	{	};

	{
		//must_be_subscriptable<int[]>    a; // OK, int*可以按下标方式访问
		must_be_subscriptable<int*>     b; // OK, int*可以按下标方式访问
		must_be_subscriptable<subs>     c; // OK, subs可以按下标方式访问
		//	must_be_subscriptable<not_subs> d; // Error, not_subs不可以按下标方式访问，编译错误
		must_be_subscriptable<std::vector<int>> e; // OK, std::vector 也可以按下标方式访问
	}
	{
		//must_be_subscriptable_as_decayable_pointer<int[]>    a; // OK, int*可以按下标方式访问
		must_be_subscriptable_as_decayable_pointer<int*>     b; // OK, int*可以按下标方式访问
		//	must_be_subscriptable_as_decayable_pointer<subs>     c; // Error, subs是重载[]的，并不是原生指针
		//	must_be_subscriptable_as_decayable_pointer<not_subs> d; // not_subs不可以按下标方式访问，编译错误
		//	must_be_subscriptable_as_decayable_pointer<std::vector<int>> e; // Error, vector是重载[]的，并不是原生指针
	}
}

void test_have_same_size()
{
	must_be_same_size<int, int>   a; // Ok
	must_be_same_size<int, long>  b; // Ok or Error（依赖于硬件架构或编译器）
	must_be_same_size<int, void>  c; // Ok , 模板偏特化，下同
	must_be_same_size<void, int>  d; // Ok
	must_be_same_size<void, void> e; // Ok
}

int main()
{
	test_type_pod();
	test_have_inherit();
	test_be_subscriptable();
	test_have_same_size();

	system("pause");
	return 0;
}