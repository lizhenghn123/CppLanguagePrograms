//deuqe:双端队列。对其两端的操作效率类似于list，其下标操作具有接近vector的效率
//	  因此deque一般用在那些需要由两端加入和删除的地方
//	  deque大部分操作类似于vector 和list
#include <iostream>
#include <deque>
using namespace std;

void printDeque(deque<int> d)
{
	//使用下标
	//for (unsigned int i = 0; i < d.size(); i++)
	//{
	// cout<<"d["<<i<<"] = "<<d[i]<<", ";
	//}

	//使用迭代器
	//deque<int>::iterator iter = d.begin();
	//for (;iter != d.end(); iter ++)
	//{
	// cout<<"d["<<iter-d.begin()<<"] = "<<(*iter)<<", ";
	//}

	//使用迭代器指针
	deque<int>::iterator *pIter = new deque<int>::iterator;
	if ( NULL == pIter )
		return ;

	for (*pIter = d.begin(); *pIter != d.end(); (*pIter)++)
		cout<<"d["<<*pIter - d.begin() <<"]="<<**pIter<<", ";

	if (NULL != pIter)
	{
		delete pIter;
		pIter = NULL;
	}
	cout<<endl;
}

template <class T>
class print_obj
{
public:
	void operator () (T& t)
	{
		cout << t << " ";
	}
};

int main()
{
	//------------------------------splice---------------------------------

	//创建deque
	deque<int> d1; //创建一个没有任何元素的deque对象
	deque<int> d2(10);//创建一个具有10个元素的deque对象，每个元素值为默认
	deque<double> d3(10, 5.5); //创建一个具有10个元素的deque对象,每个元素的初始值为5.5
	deque<double> d4(d3); //通过拷贝一个deque对象的元素值, 创建一个新的deque对象
	int iArray[] = {11, 13, 19, 23, 27};
	deque<int> d5(iArray, iArray+5);//将迭代器区间[first, last)所指的元素拷贝到一个新创建的deque对象中

	//初始化赋值：同vector一样,使用尾部插入函数push_back()
	for (int i = 1; i < 6 ; i++)
		d1.push_back(i*10);
	//遍历元素: 1-下标方式 2-迭代器方式 反向遍历（略）
	cout<<"printDeque(d1) : "<<endl;
	printDeque(d1);

	//元素插入：尾部插入用push_back()，头部插入用push_front()，其它位置插入用insert(&pos, elem)
	cout<<"d1.push_front(100): "<<endl;
	d1.push_front(100);
	printDeque(d1);
	cout<<"d1.insert(d1.begin()+3, 200): "<<endl; //支持随机存取(即[]操作符)，所以begin()可以+3
	d1.insert(d1.begin()+2,200);
	printDeque(d1);

	//元素删除 尾部删除用pop_back();头部删除用pop_front();
	//任意迭代位置或迭代区间上的元素删除用erase(&pos)/erase(&first, &last)；删除所有元素用clear();
	cout<<"d1.pop_front(): "<<endl;
	d1.pop_front();
	printDeque(d1);

	cout<<"d1.erase(d1.begin()+1): "<<endl;
	d1.erase(d1.begin()+1); //删除第2个元素d1[1]
	printDeque(d1);

	cout<<"d1.erase(d1.begin(), d1.begin() + 2) = "<<endl;
	d1.erase(d1.begin(), d1.begin() + 2);
	printDeque(d1);

	cout<<"d1.clear() :"<<endl;
	d1.clear();
	printDeque(d1);

	//其它常用
	cout<<"其它常用用法: "<<endl;
	int flag = 0;
	while(flag < 2)
	{
		if (0 == flag )
		{
			for (int i = 1; i < 6 ; i++) //恢复
				d1.push_back(i*10);
		}
		else
		{
			d1.clear();
			cout<<"after d1.clear() , d1.front(), d1.back() is abnormal! other info.:"<<endl;
		}
		cout<<"d1.empty() = "<<d1.empty()<<endl;
		cout<<"d1.size() = "<<d1.size()<<endl;
		cout<<"d1.max_size() = "<<hex<<d1.max_size()<<endl;
		if (!d1.empty())
		{
			cout<<"d1.front() = "<<d1.front()<<endl;
			cout<<"d1.back() = "<<d1.back()<<endl;
		}

		flag++;
	}

	//交换
	cout<<"d1.swap(d5)= "<<endl;
	d1.swap(d5);
	cout<<"d1 = ";
	printDeque(d1);
	cout<<"d5 = ";
	printDeque(d5);
	//printDeque(d)

	system("pause");
	return 0;
}

//说明
//#include <deque>deque容器类与vector类似，支持随机访问和快速插入删除，它在容器中某一位置上的操作所花费的是线性时间。
//与vector不同的是，deque还支持从开始端插入数据：push_front()。
//构造：
//	deque<Elem> c 创建一个空的deque 　　deque<Elem> c1(c2) 复制一个deque。 　　
//	deque<Elem> c(n) 创建一个deque，含有n个数据，数据均已缺省构造产生。 　　
//	deque<Elem> c(n, elem) 创建一个含有n个elem拷贝的deque 　　
//	deque<Elem> c(beg,end) 创建一个以[beg;end)区间的deque 　　
//	c.~deque<Elem>() 销毁所有数据，释放内存
//方法：
//	c.assign(beg,end) 将[beg; end)区间中的数据赋值给c。 　
//	c.assign(n,elem) 将n个elem的拷贝赋值给c。 　　
//	c. at(idx) 传回索引idx所指的数据，如果idx越界，抛出out_of_range。 　　
//	c.back() 传回最后一个数据，不检查这个数据是否存在。 　　
//	c.begin() 传回迭代器重的可一个数据。 　　
//	c.clear() 移除容器中所有数据。 　　
//	c.empty() 判断容器是否为空。 　　
//	c.end() 指向迭代器中的最后一个数据地址。 　　
//	c.erase(pos) 删除pos位置的数据，传回下一个数据的位置。 　　
//	c.erase(beg,end) 删除[beg,end)区间的数据，传回下一个数据的位置。 　　
//	c.front() 传回第一个数据。 　　
//	get_allocator 使用构造函数返回一个拷贝。 　　
//	c.insert(pos,elem) 在pos位置插入一个elem拷贝，传回新数据位置 　　
//	c.insert(pos,n,elem) 在pos位置插入>n个elem数据。无返回值 　　
//	c.insert(pos,beg,end) 在pos位置插入在[beg,end)区间的数据。无返回值 　　
//	c.max_size() 返回容器中最大数据的数量。 　　
//	c.pop_back() 删除最后一个数据。 　　
//	c.pop_front() 删除头部数据。 　　
//	c.push_back(elem) 在尾部加入一个数据。 　　
//	c.push_front(elem) 在头部插入一个数据。 　
//	c.rbegin() 传回一个逆向队列的第一个数据。 　　
//	c.rend() 传回一个逆向队列的最后一个数据的下一个位置。 　　
//	c.resize(num) 重新指定队列的长度。 　　
//	c.size() 返回容器中实际数据的个数。 　　
//	c.swap(c2) 　　
//	swap(c1,c2) 将c1和c2元素互换。