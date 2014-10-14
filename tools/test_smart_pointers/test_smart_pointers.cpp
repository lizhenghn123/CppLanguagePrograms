#include <iostream>
#include <vector>
#include <assert.h>
#include "smart_ptr/smart_ptr.h"
using namespace std;
using namespace zl;

class Test
{
public:
	Test() { cout << this << "\tTest\n"; }
	~Test() { cout << this << "\t~Test\n"; }
	void print() { cout << this << "\tprint\n"; }
	virtual void foo() { cout << this << "\tfoo\n"; }
};

class TestChild : public Test
{
public:
	TestChild() { cout << this << "\tTestChild\n"; }
	~TestChild() { cout << this << "\t~TestChild\n"; }
	void print() { cout << this << "\tTestChild print\n"; }
	virtual void foo() { cout << this << "\tTestChild foo\n"; }
};

void test_scoped_ptr()
{
	{
		int *p = new int(123);
		zl::unique_ptr<int> sp(p);
		p = sp.release();
		assert(p && !sp);

		int *q = NULL;
		sp.reset(q);
		assert(sp.get()==NULL);
	}
	cout << "----------------------------------------\n";
	{
		std::vector<zl::unique_ptr<Test> > vec;
		zl::unique_ptr<Test> sp(new Test);
		//zl::unique_ptr<Test> sp2 = sp;
		vec.push_back(std::move(sp));
	    //vec.push_back(new Test);
		vec[0]->print();
		//t1->print();
	}
	cout << "----------------------------------------\n";
	{
		Test *t1 = new Test;
		zl::unique_ptr<Test> sp(t1);
		sp->print();
		assert(sp == t1);
		Test *t2 = new Test;
		sp.reset(t2);
		assert(sp.get()==t2);
	}
	cout << "----------------------------------------\n";
	{
		unique_arr<Test> sp(new Test[3]);
		sp[0].print();
	}
	cout << "----------------------------------------\n";
}

void test_share_ptr()
{
	{
		Test *t1 = new Test;
		zl::shared_ptr<Test> sp1(t1);
		zl::shared_ptr<Test> sp2(sp1);
		//shared_ptr<Test> sp3(t1);
		t1->print();
		sp1->print();
		sp2->print();

		cout << t1 << "\t" << sp1 << "\t" << sp2 << "\n";
	}
	cout << "----------------------------------------\n";
	{
		Test *t1 = new TestChild;
		const zl::shared_ptr<Test> sp1(t1);
		zl::shared_ptr<TestChild> sp2 = static_pointer_cast<TestChild, Test>(sp1);
		t1->foo();
		sp1->foo();
		sp2->foo();
		cout << t1 << "\t" << sp1 << "\t" << sp2 << "\n";
	}
	cout << "----------------------------------------\n";
}

void test_weakptr()
{
	{
		Test *t1 = new Test;
		zl::shared_ptr<Test> sp1(t1);
		zl::shared_ptr<Test> sp2(sp1);
		zl::weak_ptr<Test> wp1(sp1);
		t1->print();
		sp1->print();
		//wp1->print();
	}
	cout << "----------------------------------------\n";
}
int main()
{
	test_scoped_ptr();
	test_share_ptr();
	test_weakptr();

	system("pause");
	return 0;
}