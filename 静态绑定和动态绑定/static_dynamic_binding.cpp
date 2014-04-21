#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

#define MATH_PI (3.14259)

class Figure
{
public:
	Figure(){	}

	/*virtual*/ double Area()const
	{
		return 0.0;
	}
};

class Circle:public Figure
{
public:
	Circle(double radius)
	{
		radius_ = radius;
	}
	
	double Area()const
	{
		return MATH_PI*radius_*radius_;
	}
private:
	double radius_;
};

class Rectangle:public Figure
{
public:
	Rectangle(double length,double width)
	{
		length_ = length;
		width_ = width;
	}
	
	double Area()const
	{
		return length_*width_;
	}
private:
	double length_;
	double width_;
};

/*
形参firgure所执行的操作静态绑定到Figure类的Area()方法上.这样访问的只是从基类中的Area()方法.而不是所传递过来实例所在直接类的Area()方法.所以输出结果不正确. 

解决这个问题办法就是多态,或者是动态绑定,这个动态绑定是在运行时发生的.因此又被称为 
运行时多态. 
在C++中实现动态绑定要求成员函数用virtual来标记. 
*/
void printArea(Figure &figure)
{
	std::cout<<"面积是: "<<figure.Area()<<"\n";
}
void printArea(Figure *figure)
{
	std::cout<<"面积是: "<<figure->Area()<<"\n";
}

class A
{
public:
	/*virtual*/ void func(){ std::cout << "A::func()\n"; }
};
class B : public A
{
public:
	void func(){ std::cout << "B::func()\n"; }
};
class C : public A
{
public:
	void func(){ std::cout << "C::func()\n"; }
};

class E
{
public:
	virtual void func(int i = 0)
	{ 
		std::cout << "E::func()\t"<< i <<"\n";
	}
};
class F : public E
{
public:
	virtual void func(int i = 1)
	{
		std::cout << "F::func()\t" << i <<"\n";
	}
};


int main(void)
{
	F* pf = new F();
	E* pe = pf;
	pf->func(); //F::func() 1  正常，就该如此；
	pe->func(); //F::func() 0  哇哦，这是什么情况，调用了子类的函数，却使用了基类中参数的默认值！
	

	C* pc = new C(); //pc的静态类型是它声明的类型C*，动态类型也是C*；
	B* pb = new B(); //pb的静态类型和动态类型也都是B*；
	A* pa = pc;      //pa的静态类型是它声明的类型A*，动态类型是pa所指向的对象pc的类型C*；
	pa = pb;         //pa的动态类型可以更改，现在它的动态类型是B*，但其静态类型仍是声明时候的A*；
	C *pnull = NULL; //pnull的静态类型是它声明的类型C*,没有动态类型，因为它指向了NULL；

	pa->func();      //A::func() pa的静态类型永远都是A*，不管其指向的是哪个子类，都是直接调用A::func()；
	pc->func();      //C::func() pc的动、静态类型都是C*，因此调用C::func()；
	pnull->func();   //C::func() 不用奇怪为什么空指针也可以调用函数，因此这在编译期就确定了，和指针空不空没关系；

	//如果注释掉类C中的void func()函数
	pa->func();      //A::func() 理由同上；
	pc->func();      //A::func() pc在类C中找不到func的定义，因此到其基类中寻找；
	pnull->func();   //A::func()，原因也解释过了；

	//如果为A中的void func()函数添加virtual特性
	pa->func();      //B::func() 因为有了virtual虚函数特性，pa的动态类型指向B*，因此先在B中查找，找到后直接调用；
	pc->func();      //C::func() pc的动、静态类型都是C*，因此也是先在C中查找；
	pnull->func();   //空指针异常，因为是func是virtual函数，因此对func的调用只能等到运行期才能确定，然后才发现pnull是空指针；
	
	
	std::cout << "------------------------------\n";

	Figure figure;
	Circle circle(2.0);
	Rectangle rect(3.0,4.0);

	std::cout<<"Figure的面积是: "<<figure.Area()<<"\n";
	std::cout<<"Circle的面积是: "<<circle.Area()<<"\n";
	std::cout<<"Rectangle的面积是: "<<rect.Area()<<"\n";
	std::cout << "------------------------------\n";

	printArea(figure);
	printArea(circle);
	printArea(rect);
	std::cout << "------------------------------\n";

	printArea(&figure);
	printArea(&circle);
	printArea(&rect);
	std::cout << "------------------------------\n";

	system("pause");
	return 0;
}

