#include<iostream>
#include<map>
#include<string>
#include<list>
#include <functional>
#include <xfunctional>
#include <memory>
#include <xmemory>
#include <algorithm>
#include <iterator>
#include <string.h>
using namespace std;

class Test
{
public:
	int k;
	Test(int i):k(i){}
	int get(){return k;}
};
bool operator<(Test a,Test b)
{
	return a.get()<b.get();
}
map< int, list< Test * >, less< int > > _m_List;

void Add( Test *_ele, int _priority = 1000 )
{
	list< Test * > &rl = _m_List[_priority];
	rl.remove( _ele );
	rl.push_back( _ele );
}

//自己定义升降序，类似于stl中的less<T> great<T>
template<typename T>
struct bi_key_map_sort_down: public binary_function<const T&,const T&,bool>
{
public:
	bool operator()(const T& p1,const T& p2)const
	{
		return p2 < p1;//return p1 < p2;
	}
};
//template<typename T>
struct bi_key_map_nocase: public binary_function<const string&,const string&,bool>
{
public:
	bool operator()(const string& p1,const string& p2)const
	{
		const char *s1=p1.c_str();
		const char *s2=p2.c_str();
		int r=stricmp(s1,s2);
		if(r<=0)
			return false;
		return true;
	}
};

void testSort()
{
	map<string,int,bi_key_map_sort_down<string> >mmm;
//	map<string,int,less<string> > mmm;
	mmm.insert(make_pair("str1",1));
	mmm.insert(make_pair("str1",435));
	mmm.insert(make_pair("yewr",34));
	mmm.insert(make_pair("arf",67));
}

void testNocase()
{
	map<string,int,bi_key_map_nocase/*<string>*/ > mc;
	mc.insert(make_pair("str1",1));
	mc.insert(make_pair("str2",435));
	mc.insert(make_pair("STR1",34));
	mc.insert(make_pair("arf",67));

	if(mc.find("STR1")!=mc.end())
	{
		int i = 0;
	}
}

int main()
{
	testSort();
	testNocase();
	string s1 = "s1";
	string s2 = "s2";
	map<int,string> m;
	m[100]=s1;
	m[100]=s2;
	for(map<int,string>::iterator it = m.begin();it!=m.end();it++)
		cout<<it->first<<"   "<<it->second<<"\n";
	m.erase(m.find(100));

	Test * t1 = new Test(100);
	Test * t2 = new Test(200);
	Test * t3 = new Test(300);
	Test * t4 = new Test(400);
	Test * t5 = new Test(500);
	Test * t6 = new Test(600);
	//以下是测试viwo中使用map的一段代码
	Add(t1,900);
	Add(t2,3000);
	Add(t3,900);

	//_m_List中key值是int型，而value值是 list< Test * >型
	cout<<_m_List.size()<<endl;
	for(map< int, list< Test * >, less< int > >::iterator it = _m_List.begin();it!=_m_List.end();it++)
	{
		cout<<it->first<<"   ";
		list< Test * > t = it->second;
		for(list<Test *>::iterator listit = t.begin();listit!=t.end();listit++)
			cout<<(*listit)->get()<<"   "<<*listit<<"\n";
	}

	//map默认按 '<' 号排序，即要求key必须提供 '<' 比较操作，当然也可以使用'>'或者其他比较操作
	cout<<"map默认按 '<' 号排序，当然也可以使用'>'或者其他比较操作"<<endl;
	map<int,Test*,greater<int>> mgr;
	mgr[1] = t1;
	mgr[2] = t2;
	mgr[3] = t3;
	mgr[4] = t4;
	mgr[5] = t5;
	mgr[6] = t6;
	for(map<int,Test*,greater<int>>::iterator it = mgr.begin();it!=mgr.end();it++)
		cout<<it->first<<"   "<<it->second->get()<<"\n";

	//map 默认要求key必须提供 '<' 比较操作，
	cout<<"map 默认要求key必须提供 '<' 比较操作"<<endl;
	map<Test,int> mti;
	Test a(2);
	Test b(3);
	Test c(4);
	Test d(5);
	mti[c] = 444;
	mti[d] = 555;
	mti[a] = 222;
	mti[b] = 333;

	for(map<Test,int>::const_iterator it = mti.begin();it!=mti.end();it++)
		//cout<<(it->first).get()<<"   "<<it->second<<"\n";
		cout<<it->second<<"\n";
	getchar();
	return 0;
}