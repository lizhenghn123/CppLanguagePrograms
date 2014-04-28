#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <array>
#include "HighPrecisionTime.h"


const int MAX_SIZE = 500000;

std::array<int, MAX_SIZE> myArr;
std::list<int> myList;
std::vector<int> myVec;

/****************************************************************
* 此代码来自 http://www.cnblogs.com/clayman/p/3654768.html
* 以证明即使是有频繁插入操作，vector也不必list差，原因：
*   1. list需要占用更多内存，其次vector总是保证元素位于连续的内存，这是最重要的！
*   2. Cache missing导致的性能损失甚至比复制元素还严重。
*   3. 对现代CPU来说，运算速度已经非常快，一次cache missing就会浪费n个cpu周期，合理组织数据，让cpu减少等待时间是现代cpu非常重要的优化手段。
* 注意，上面的演示代码只是为了展示cache missing的重要性，并不是完成这个任务的最优方法，另外实际情况下对于复杂类型来说，随着复制代价的提高，vector未必就能总胜出了:)。
****************************************************************/
void test()
{
	//生成MAX_SIZE个随机数
	for(int i = 0; i < MAX_SIZE; i++)
		myArr[i] = rand() % 2000;

	//预先分配空间
	myVec.reserve(MAX_SIZE);
	myList.resize(MAX_SIZE,10000);

	std::cout << std::setprecision(10);

	HighPrecisionTime watch;
	watch.Reset();

	//将随机数插入到vector中，且保持排序状态
	for(int i = 0; i < MAX_SIZE; i++)
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
	std::cout << watch.ElapsedTimeInMill()<<"毫秒\n";
	
	watch.Reset();
	//将随机数插入到list中，且保持排序状态
	for(int i = 0; i < MAX_SIZE; i++)
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
	std::cout << watch.ElapsedTimeInMill()<<"毫秒\n";
}

int main()
{
	test();
	system("pause");
	return 0;
};