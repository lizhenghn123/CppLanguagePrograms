#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
using namespace std;

class Int
{
public:
	Int(int i=0):m_i(i){}
	void print()const
	{
		cout<<"[ "<<m_i<<" ]\n";
	}
	~Int(){}
	int m_i;
};
void print(int i)
{
	cout<<i<<"\t";
}
template<typename T>
struct myprint
{
public:
	void operator()(const T& t)
	{
		cout<<t<<"\t";
	}
};
void test1()
{
	int ai[6] = {0,1,2,3,4,5};
	vector<int> vec(ai,ai+6);

	//此处的myprint<int>()是一个临时对象，而不是一个函数调用
	//这个对象被传入for_each中，当for_each结束时该临时对象的生命周期也就结束了
	for_each(vec.begin(),vec.end(),myprint<int>());
	cout<<endl;
}
void test2()
{
	ostream_iterator<int> outite(cout,"\t");

	int ai[6] = {2,21,12,7,19,23};
	vector<int> vec(ai,ai+6);

	//找出不小于 12 的元素个数
	cout<< count_if(vec.begin(),vec.end(),not1(bind2nd(less<int>(),12))) << endl;

	//将每个元素v执行（v+2)*3,然后输出到outite
//	transform(vec.begin(),vec.end(),outite,compose1(bind2nd(multiplies<int>(),3),bind2nd(plus<int>(),2) )) <<endl;

	//以下将所有元素输出，有多种方法
	//1.输出到outite
	copy(vec.begin(),vec.end(),outite);		cout<<endl;
	//for_each的第三个参数（函数指针或函数对象）只能带一个参数
	//2.以函数指针
	for_each(vec.begin(),vec.end(),print);	cout<<endl;
	//3.以修饰过的一般函数
	for_each(vec.begin(),vec.end(),ptr_fun(print));	cout<<endl;
	//4.以函数对象
	for_each(vec.begin(),vec.end(),myprint<int>());	cout<<endl;

	Int t1(1),t2(2),t3(3),t4;
	vector<Int> vv;
	vv.push_back(t1);
	vv.push_back(t2);
	vv.push_back(t3);
	vv.push_back(t4);
	//5.以修饰过的成员函数
	for_each(vv.begin(),vv.end(),mem_fun_ref(&Int::print));
}
//template<typename T>
struct del_sequence_ptr //: public std::unary_function<T,void>
{
public:
	template<typename T>
	void operator()(T *t)
	{
		delete t;
	}
};
void test3()
{
	typedef std::vector<Int*>			IntContainer;
	//vector<Int*> vec;
	IntContainer vec;
	vec.push_back(new Int);
	vec.push_back(new Int(178));
	for_each(vec.begin(),vec.end(),del_sequence_ptr());

	vector<string*> vs;
	vs.push_back(new string("df"));
	for_each(vs.begin(),vs.end(),del_sequence_ptr());
}
int main()
{
	test1();
	test2();
	test3();
	system("pause");
	return 0;
}