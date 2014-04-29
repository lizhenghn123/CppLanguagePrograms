#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <list>
#include <array>
#include "HighPrecisionTime.h"

template < typename T>
std::string ToStr(const T& t)
{
	std::ostringstream oss;
	oss.str("");
	oss << t;
	return oss.str();
}

const int MAX_SIZE = 100000;

//只是指针时，明显vector插入更快
std::list<int> list1;
std::vector<int> vec1;
std::array<int, MAX_SIZE> myArr1;
void test_insert_int()
{


	for (int i = 0 ; i< MAX_SIZE; i++)
	{
		myArr1[i] = i+1;
	}

	HighPrecisionTime watch;

	watch.Reset();
	for (int i = 0 ; i< MAX_SIZE; i++)
	{
		list1.push_back(myArr1[i]);
	}
	std::cout<<"insert " << MAX_SIZE << " ints to list,   cost "<<watch.ElapsedTimeInMill()<<" ms\n";

	watch.Reset();
	for (int i = 0 ; i< MAX_SIZE; i++)
	{
		vec1.push_back(myArr1[i]);
	}
	std::cout<<"insert " << MAX_SIZE << " ints to vector, cost "<<watch.ElapsedTimeInMill()<<" ms\n";
}

//当是string等拷贝代价比较大时，明显list插入更快，因为此时vector的拷贝移动代价更大
std::list<std::string> list2;
std::vector<std::string> vec2;
std::array<std::string, MAX_SIZE> myArr2;
void test_insert_string()
{
	for (int i = 0 ; i< MAX_SIZE; i++)
	{
		myArr2[i] = ToStr(i+1);
	}

	HighPrecisionTime watch;

	watch.Reset();
	for (int i = 0 ; i< MAX_SIZE; i++)
	{
		list2.push_back(myArr2[i]);
	}
	std::cout<<"insert " << MAX_SIZE << " strings to list,   cost "<<watch.ElapsedTimeInMill()<<" ms\n";

	watch.Reset();
	for (int i = 0 ; i< MAX_SIZE; i++)
	{
		vec2.push_back(myArr2[i]);
	}
	std::cout<<"insert " << MAX_SIZE << " strings to vector, cost "<<watch.ElapsedTimeInMill()<<" ms\n";
}

/****************************************************************
* 此函数来自 http://www.cnblogs.com/clayman/p/3654768.html
* 以证明即使是有频繁插入操作，vector也不必list差，原因：
*   1. list需要占用更多内存，其次vector总是保证元素位于连续的内存，这是最重要的！
*   2. Cache missing导致的性能损失甚至比复制元素还严重。
*   3. 对现代CPU来说，运算速度已经非常快，一次cache missing就会浪费n个cpu周期，合理组织数据，让cpu减少等待时间是现代cpu非常重要的优化手段。
* 注意，下面的演示代码只是为了展示cache missing的重要性，并不是完成这个任务的最优方法，另外实际情况下对于复杂类型来说，随着复制代价的提高，vector未必就能总胜出了:)。
****************************************************************/
std::array<int, MAX_SIZE> myArr;
std::list<int> myList;
std::vector<int> myVec;
void test_insert_int_keep_order()
{
	for(int i = 0; i < MAX_SIZE; i++)
		myArr[i] = rand() % 2000;

	//预先分配空间
	myVec.reserve(MAX_SIZE);
	myList.resize(MAX_SIZE,10000);

	std::cout << std::setprecision(10);

	HighPrecisionTime watch;

	watch.Reset();	
	for(int i = 0; i < MAX_SIZE; i++)  //将随机数插入到list中，且保持排序状态
	{
		int value = myArr[i];
		auto it = myList.begin();
		for( ; it != myList.end(); it++)
		{
			if(*it > value)
			{
				it = myList.insert(it, value);
				break;
			}
		}
		if(it == myList.end())
			myList.push_back(value);
	}
	std::cout<<"insert " << MAX_SIZE << " ints to list, and keep order,   cost "<<watch.ElapsedTimeInMill()<<" ms\n";

	watch.Reset();
	for(int i = 0; i < MAX_SIZE; i++)  //将随机数插入到vector中，且保持排序状态
	{
		int value = myArr[i];
		auto it = myVec.begin();
		for( ; it != myVec.end(); it++)
		{
			if(*it > value)
			{
				it = myVec.insert(it, value);
				break;
			}
		}
		if(it == myVec.end())
			myVec.push_back(value);
	}
	std::cout<<"insert " << MAX_SIZE << " ints to vector, and keep order, cost "<<watch.ElapsedTimeInMill()<<" ms\n";
}


int main()
{
	std::cout<<"--------------------------------------------\n";
	test_insert_int();

	std::cout<<"--------------------------------------------\n";
	test_insert_string();

	std::cout<<"--------------------------------------------\n";
	//test_insert_int_keep_order();

	system("pause");
	return 0;
}

/*************************************************
Output(Debug)
第一次：
--------------------------------------------
insert 10000 ints to list,   cost 29.1224 ms
insert 10000 ints to vector, cost 4.66314 ms
--------------------------------------------
insert 10000 strings to list,   cost 44.7178 ms
insert 10000 strings to vector, cost 695.75 ms
--------------------------------------------
第二次：
--------------------------------------------
insert 100000 ints to list,   cost 241.752 ms
insert 100000 ints to vector, cost 37.4099 ms
--------------------------------------------
insert 100000 strings to list,   cost 422.486 ms
insert 100000 strings to vector, cost 16859.6 ms
--------------------------------------------

Output(Release)
第一次：
--------------------------------------------
insert 10000 ints to list,   cost 10.5768 ms
insert 10000 ints to vector, cost 0.209614 ms
--------------------------------------------
insert 10000 strings to list,   cost 20.4362 ms
insert 10000 strings to vector, cost 1.39453 ms
--------------------------------------------
第二次：
--------------------------------------------
insert 100000 ints to list,   cost 84.6288 ms
insert 100000 ints to vector, cost 1.98792 ms
--------------------------------------------
insert 100000 strings to list,   cost 66.9376 ms
insert 100000 strings to vector, cost 10.6645 ms
--------------------------------------------
*************************************************/