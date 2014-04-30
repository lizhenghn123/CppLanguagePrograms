//stl提供了两种容器：序列（vector、list、deque）和关联容器（map、mutilmap、set、mutilset）
//而 stack和queue是作为基本容器的适配器。
////-----------------------------------序  列--------------------------------

////-----------------------------------适配器--------------------------------
//适配器提供的事原来容器的一个受限的界面，特别是容器不提供迭代器
//比如：stack默认使用一个deque保存自己的元素，但也可以采用任何提供了back()、push_back()和pop_back()的序列
//		 例如： stack<int> s1; //用deque<int>保存数据
//				stack<int,vector<int> > s2;////用vector<int>保存数据
//	  queue默认使用一个deque保存自己的元素，但也可以采用任何提供了front()、back()、push_back()和pop_front()的序列
//		  但vector没有提供pop_front()操作，所以vector不能用作queue的基础容器
//	  priority_queue 默认使用一个vector保存自己的元素，但也可以采用任何提供了front()、push_back()和pop_back()的序列
//		  其本身也是一种队列，其中的每一个元素被给定了一个优先级，以此来控制元素到达top（）的顺序
////---------------------------------关联容器--------------------------------
//set, multiset, map, multimap  是一种非线性的树结构，具体的说采用的是一种比较高效的特殊的平衡检索二叉树—— 红黑树结构
//set  又称集合，实际上就是一组元素的集合，但其中所包含的元素的值是唯一的，且是按一定顺序排列的，集合中的每个元素被称作
//	 集合中的实例。因为其内部是通过链表的方式来组织，所以在插入的时候比vector  快，但在查找和末尾添加上比vector慢。
//multiset  是多重集合，其实现方式和set  是相似的，只是它不要求集合中的元素是唯一的，也就是说集合中的同一个元素可以出现多次。
//map  提供一种“键- 值”关系的一对一的数据存储能力。其“键”在容器中不可重复，且按一定顺序排列（其实我们可以将set  也看成是
//	 一种键- 值关系的存储，只是它只有键没有值。它是map 的一种特殊形式）。由于其是按链表的方式存储，它也继承了链表的优缺点。
//multimap  和map 的原理基本相似，它允许“键”在容器中可以不唯一。

//vector、list、deque选择规则
//	  1、如果需要高效的随即存取，而不在乎插入和删除的效率，使用vector
//	  2、如果需要大量的插入和删除，而不关心随即存取，则应使用list
//	  3、如果需要随即存取，而且关心两端数据的插入和删除，则应使用deque。

//map、mutilmap、set、mutilset选择规则

//stack模板类的定义在<stack>头文件中
//	stack模板类需要两个模板参数，一个是元素类型，一个容器类型，但只有元素类型是必要的，在不指定容器类型时，默认的容器类型为deque。
//
//	定义stack对象的示例代码如下：
//	stack<int> s1;
//	stack<string> s2;
//
//stack的基本操作有：
//	入栈，如例：s.push(x);
//	出栈，如例：s.pop();注意，出栈操作只是删除栈顶元素，并不返回该元素。
//	访问栈顶，如例：s.top()
//	判断栈空，如例：s.empty()，当栈空时，返回true。
//	访问栈中的元素个数，如例：s.size()

#include <iostream>
#include <ostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
using namespace std;

int main ()
{
	vector<int> v1(5), v2(5), v3(5);
	iota(v1.begin(), v1.end(), 0);
	iota(v2.begin(), v2.end(), 5);
	iota(v3.begin(), v3.end(), 10);

	stack<vector<int> > s;
	s.push(v1);
	s.push(v2);
	s.push(v3);

	cout << "size of stack 's' = "
		<< s.size() << endl;

	if ( v3 != v2 )
		s.pop();

	cout << "size of stack 's' = "
		<< s.size() << endl;

	vector<int> top = s.top();
	cout << "Contents of v2 : ";

	copy(top.begin(),top.end(),	ostream_iterator<int>(cout," "));
	cout << endl;

	while ( !s.empty() )
		s.pop();

	cout << "Stack 's' is " << (s.empty() ? ""  : "not ") << "empty" << endl;

	system("pause");
	return 0;
} 