#include<iostream>
#include<list>
#include<string>
#include<algorithm>
using namespace std;

class A
{
public :
	int k;
	A(int i=100):k(i){}
};
//一元谓词
class Initial : public unary_function<char,bool>
{
	char chr;
public:
	explicit Initial(char c):chr(c){}
	bool operator()(const string& str)const { return str[0] == chr ;}
};
//二元谓词
class bi_list_equal : public binary_function<const string&,const string&,bool>
{
	char chr;
public:
	explicit bi_list_equal(char c):chr(c){}
	bool operator()(const string& s1,const string& s2)const
	{
		return s1[0]== s2[0] && s1[0] == chr;//???
	}
};
void print(const list<string>& li)
{
	cout<<li.size()<<" : ";
	list<string>::const_iterator iter;
	for( iter = li.begin(); iter != li.end(); iter++ )
		cout << *iter<<"\t";
	cout<<endl;
}

void test_unique()
{
	list<int> listint;
	listint.push_back(1);
	listint.push_back(2);
	listint.push_back(2);
	listint.push_back(3);
	listint.push_back(3);
	listint.push_back(3);
	listint.push_back(4);
	listint.push_back(6);
	list<int>::iterator first = listint.begin();
	list<int>::iterator last  = listint.end();
	list<int>::iterator next  = first;
	//unique
	while( ++next != last)
	{
		if( *first == *next)
			next = listint.erase(next);
		else
			first = next;

		next = first;
	}

	list<int> listint2;
	listint2.push_back(10);
	listint2.push_back(100);
	listint2.push_back(2);
	listint2.push_back(12);
	listint2.push_back(3);

	//listint.merge(listint2);
	cout<<"----------------------\n";
}
int main()
{
	test_unique();
	//-----------------------------------
	list<A*> li; //如果使用 list<A*>* pli;，则必须在使用前初始化pli，pli=NULL
	A *a = new A(7);
	A *aa = new A;
	li.push_back(a);
	li.push_back(aa);
	cout<<"size = "<<li.size()<<endl;
	li.erase(li.begin());  //删除list中指针并不影响原指针，原指针a仍然存在可以访问
	cout<<"size = "<<li.size()<<endl;
	cout<<a->k<<endl;
	delete a;  //删除指针后，则list中的指针失效，因为所指指针a已被删除
	cout<<"size = "<<li.size()<<endl;
	cout<<a->k<<endl;
	for(list<A*>::iterator ite = li.begin();ite!=li.end();ite++)
		cout<<"list : "<<(*ite)->k<<endl;
	cout<<"-----------------------------------"<<endl;

	list<string> fruit;
	list<string> citrus;
	fruit.push_back("apple");
	fruit.push_back("pear");
	citrus.push_back("orange");
	citrus.push_back("grapefruit");
	citrus.push_back("lemon");
	list<string> citrusTmp(citrus);

	print(fruit);
	print(citrusTmp);
	cout<<"-----------------------------------"<<endl;

	list<string>::iterator p = find_if(fruit.begin(),fruit.end(),Initial('p'));//返回一个指向第一个string首字母是‘p’的迭代器
	fruit.splice(p,citrusTmp,citrusTmp.begin());//将citrusTmp的第一个元素添加到 p之前，citrusTmp的第一个元素将从citrusTmp删去
	print(fruit);
	print(citrusTmp);
	cout<<"-----------------------------------"<<endl;

	fruit.splice(fruit.end(), citrusTmp);//将citrusTmp的所有元素添加到fruit中(添加到第一个参数之前)，之后citrusTmp中元素为空
	print(fruit);
	print(citrusTmp);
	cout<<"-----------------------------------"<<endl;

	citrusTmp = citrus;
	fruit.sort();
	citrusTmp.sort();
	fruit.merge(citrusTmp);
	print(fruit);
	print(citrusTmp);
	cout<<"-----------------------------------"<<endl;

	fruit.remove("orange");//删除所有是orange的元素
	print(fruit);
	cout<<"-----------------------------------"<<endl;

	fruit.remove_if(Initial('l'));//删除所有开头是字母l的元素
	print(fruit);
	cout<<"-----------------------------------"<<endl;

	fruit.sort();
	fruit.unique(bi_list_equal('g'));//删除开头是字母p的重复元素,
	print(fruit);

	fruit.unique();//先排序，再删除所有重复元素，之所以要先排序，是因为unique只能删除连续重复的元素
	print(fruit);
	cout<<"-----------------------------------"<<endl;

	system("pause");
	return 0;
}