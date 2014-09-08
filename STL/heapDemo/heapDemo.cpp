#include <iostream>
#include <vector>
#include <algorithm> //heap算法头文件
using namespace std;
//heap并不属于STL容器，但确是priority_queue内部实现。
//heap是一种完全二叉树，除叶节点外，所有节点都是全部填满的。叶节点按从左至右也是填满的
//heap不提供遍历功能，也没有迭代器

void print_vec_int(const vector<int>& vec)
{
	for (size_t i=0; i<vec.size(); i++)
		cout<<vec[i]<<" ";
	cout<<"\n";
}
int main()
{
	int ia[9] = {0,1,2,3,4,8,10,3,5};
	
	
	{
		// 测试以vector作为容器
		vector<int> ivec(ia,ia+9);

		print_vec_int(ivec);

		make_heap(ivec.begin(),ivec.end());
		print_vec_int(ivec);

		ivec.push_back(7);
		push_heap(ivec.begin(),ivec.end());
		print_vec_int(ivec);

		pop_heap(ivec.begin(),ivec.end());
		cout<<ivec.back()<<"\n";		//return 9，但是并不移出9
		ivec.pop_back();		//no return，移除 9 
		cout<<ivec.back()<<"\n"; //必须先pop_heap
		print_vec_int(ivec);

		sort_heap(ivec.begin(),ivec.end());
		print_vec_int(ivec);
	}

	{
		// 测试以数组作为容器
		make_heap(ia,ia+9); //因为数组无法改变大小，因此不能进行push_heap操作

		sort_heap(ia,ia+9); //排序之后，不再是个合法的heap
		for(int i=0;i<9;i++)
			cout<<ia[i]<<" ";
		cout<<"\n";

		//重新再做一个heap
		make_heap(ia,ia+9);

		pop_heap(ia,ia+9);
		cout<<ia[8]<<"\n";
	}
	system("pause");
	return 0;
}