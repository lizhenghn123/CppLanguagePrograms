#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

template<class T> class Sum
{
	T res;
public:
	Sum(T i=0):res(i){}  //初始化
	void operator()(T x){ res +=x; }//累加s
	T result()const{ return res ;}//返回和数
};

class A
{
public:
	A(int k = 0):i(k){}
	~A()
	{
		cout<<i<<"\t~A()\n";
	}
	int i;
};
class B
{
public:
	B(int k = 0):i(k){}
	~B()
	{
		cout<<i<<"\t~B()\n";
	}
	int i;
};
struct id_lod_model
{
	int id;
	int lod;
	string *pstr;
	id_lod_model(int i,int l,string* p):id(i),lod(l),pstr(p){}
	~id_lod_model()
	{
		cout<<"~id_lod_model\t"<<id<<"\t"<<lod<<"\t"<<*pstr<<endl;
	}
};
struct del_li
{
public :
	void operator()(A *a)
	{
		delete a;
	}
};
template<typename T1,typename T2>
struct del_map
{
public :
	//void operator()(pair<int,A*> p)
	//{
	//	delete p.second;
	//}
	template<typename T1,typename T2>
	void operator()(pair<T1,T2*> p)
	{
		delete p.second;
	}
};

class map_find_second
{
	pair<id_lod_model*,int> p;
	int maxtime;
public:
	map_find_second(int i):maxtime(i){}
	void operator()(pair<id_lod_model*,int> _p)
	{
		if(_p.second<maxtime)
		{
			p = _p;
			maxtime = _p.second;
		}
	}
	pair<id_lod_model*,int> result()
	{
		return p;
	}
};
void testMapFind()
{
	typedef int time;
	vector<id_lod_model*> vec;
	map<id_lod_model*,int> m_cache;//根据指针地址排序
	int size = 3;
	for(int i = 0;i<size;i++)
		vec.push_back(new id_lod_model(i,i*2,new string("m_cache!!!")));
	for(int i = 0;i<size;i++)
	{
		m_cache[vec[i]] = rand()%100;
	}
	if(m_cache.find(vec[0])!=m_cache.end())//根据指针地址比较
		int i = -1;
	int maxtime = 100;
	//map<id_lod_model*,int>::iterator iter = find_if(m_cache.begin(),m_cache.end(),map_find_second(maxtime));
	map_find_second res(maxtime);
	res = for_each(m_cache.begin(),m_cache.end(),res);

	//delete res.result().first;
	map<id_lod_model*,int>::iterator iter1 = m_cache.find(res.result().first);
		if(iter1!=m_cache.end())
		{
			delete iter1->first;
			m_cache.erase(iter1);
		}
	//delete iter->first;
	//m_cache.erase(iter);
}
int main()
{
	testMapFind();

	list<double> ld;
	ld.push_back(2.0);
	ld.push_back(-1.5);
	ld.push_back(100.0);
	ld.push_back(3.0);

	Sum<double>s;
	s = for_each(ld.begin(),ld.end(),s) ; //对ld的每个元素调用s（）
	cout<<"总和是 "<< s.result()<<endl;
	
	list<A*> la;
	//list<A*> la;
	la.push_back(new A);
	la.push_back(new A(4));
	la.push_back(new A(3));
	la.push_back(new A(1));
	std::for_each(la.begin(),la.end(),del_li());
	cout<<"-----------------------------------------\n";
	
	map<int,A*> mmm;
	mmm[1] = new A;
	mmm[2] = new A(2);
	mmm[3] = new A(3);
	if(mmm.find(1)!=mmm.end())
		int i = -1;
	for_each(mmm.begin(),mmm.end(),del_map<int,A>() );

	map<int,B*> mmmb;
	mmmb[1] = new B;
	mmmb[2] = new B(2);
	mmmb[3] = new B(3);
	for_each(mmmb.begin(),mmmb.end(),del_map<int,B>());

	getchar();
	return 0;
}