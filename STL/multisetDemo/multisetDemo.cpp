#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

void print (multiset<int, less<int> >& s)
{
	multiset<int, less<int> >::iterator iter;
	for ( iter = s.begin(); iter != s.end(); iter++ )
		cout << *iter << " ";
	cout << endl;
}
void test_Construction()
{
	int ary[] = {1,2,3,2,5,4,2,1,4,5};

	multiset<int, less<int> > ms1;
	multiset<int, greater<int> > ms2(ary,ary+10);

	multiset<int, greater<int> >::iterator iter2;
	cout << "ms2 : ";
	for ( iter2 = ms2.begin(); iter2 != ms2.end(); iter2++ )
		cout << *iter2 << " ";
	cout << endl;

	// copy constructor
	multiset<int, greater<int> > ms3(ms2);
	cout << "ms3 : ";
	for ( iter2 = ms3.begin(); iter2 != ms3.end(); iter2++ )
		cout << *iter2 << " ";
	cout << endl;

	iter2 = ms2.begin();

	while ( iter2 != ms2.end() )
		ms1.insert(*iter2++);

	multiset<int>::iterator iter1;
	cout << "ms1 : ";
	for ( iter1 = ms1.begin(); iter1 != ms1.end(); iter1++ )
		cout << *iter1 << " ";
	cout << endl;
};
void test_Count()
{
	int ary[] = {1,2,3,2,3,4,8,2,5,6};
	multiset<int, less<int> > s;

	s.insert(ary,ary+10);
	print(s);

	int n = s.count(2);
	cout << "count of '2' is "<< n << endl;
}
void test_equal_ranges()
{
	set<int> c;

	c.insert(1);
	c.insert(2);
	c.insert(4);
	c.insert(10);
	c.insert(11);

	cout << "lower_bound(3): " 	<< *c.lower_bound(3) << endl;
	cout << "upper_bound(3): "  << *c.upper_bound(3) << endl;
	cout << "equal_range(3): "  << *c.equal_range(3).first << " " << *c.equal_range(3).second << endl;
	cout << endl;

	cout << "lower_bound(5): "  << *c.lower_bound(5) << endl;
	cout << "upper_bound(5): "  << *c.upper_bound(5) << endl;
	cout << "equal_range(5): "  << *c.equal_range(5).first << " " << *c.equal_range(5).second << endl;
}
void test_erase()
{
	int ary[] = {1,2,3,2,3,4,8,2,5,6};
	multiset<int, less<int> > s;
	s.insert(ary,ary+10);
	print(s);

	s.erase(2); // erase all '2'
	print(s);

	multiset<int, less<int> >::iterator It;
	It = s.find(5);
	s.erase(It); // erase '5'
	print(s);

	It = s.find(4);
	s.erase(It,s.end()); // erase from It to the end of multiset
	print(s);
}
void test_insert()
{
	int ary[] = {1,2,3,2,3,4,8,2,5,6};
	multiset <int> s;
	s.insert(10);
	print(s);

	s.insert(ary,ary+5);
	print(s);
	multiset<int, less<int> >::iterator It = s.begin();
	s.insert(It,20);
	print(s);
}

//--------------------------------------
template<typename T>
struct display
{
	void operator()(const T& t)
	{
		cout<<t<<" ";
	}
};
void test_set_algorithm()
{
	int ia1[6] = {1,3,5,7,9,11};
	int ia2[7] = {1,1,2,3,5,8,13};

	multiset<int> S1(ia1,ia1+6);
	multiset<int> S2(ia2,ia2+7);

	for_each(S1.begin(),S1.end(),display<int>());
	cout<<"\n";
	for_each(S2.begin(),S2.end(),display<int>());
	cout<<"\n";

	multiset<int>::iterator first1 = S1.begin();
	multiset<int>::iterator last1  = S1.end();
	multiset<int>::iterator first2 = S2.begin();
	multiset<int>::iterator last2  = S2.end();

	cout<<"Union of S1 and S2: ";
	set_union(first1,last1,first2,last2,ostream_iterator<int>(cout," "));
	cout<<"\n";

	first1 = S1.begin();
	last1  = S1.end();
	cout<<"Intersection of S1 and S2: ";
	set_intersection(first1,last1,first2,last2,ostream_iterator<int>(cout," "));
	cout<<"\n";

	first1 = S1.begin();
	last1  = S1.end();
	cout<<"Difference of S1 and S2(S1-S2): ";
	set_difference(first1,last1,first2,last2,ostream_iterator<int>(cout," "));
	cout<<"\n";

	first1 = S1.begin();
	last1  = S1.end();
	cout<<"Symmetric difference of S1 and S2: ";
	set_symmetric_difference(first1,last1,first2,last2,ostream_iterator<int>(cout," "));
	cout<<"\n";

	first1 = S2.begin();
	last1  = S2.end();
	first2 = S1.begin();
	last2  = S1.end();
	cout<<"Symmetric difference of S2 and S1(S2-S1): ";
	set_difference(first1,last1,first2,last2,ostream_iterator<int>(cout," "));
	cout<<"\n";
}

class twt
{
	std::string name;
};

struct UserInfo
{
public:
	UserInfo(string s,long l):name(s),score(l){}
	string name;
	long score;

	bool operator<(const UserInfo rhs)
	{
		return rhs.score < this->score;
	}
};
bool operator<(const UserInfo lhs,const UserInfo rhs)
{
	return lhs.score > rhs.score;
}
struct TopList
{
public:
	void RefreshTopList(UserInfo& user)
	{
		items.insert(user);
	}

	int GetRankByName(string& name)
	{
		multiset<UserInfo>::iterator iter;
		for(iter=items.begin();iter!=items.end();iter++)
		{
			if( (iter)->name == name )
				return (iter)->score;
		}
		return -1;
	}

	UserInfo* GetUserByRank(long rank)
	{
		multiset<UserInfo>::iterator iter;
		for(iter=items.begin();iter!=items.end();iter++)
		{
			UserInfo ui = *iter;
			UserInfo *pu = &ui;
			if( (iter)->score == rank )
				return const_cast<UserInfo*>(&(*iter));//(UserInfo*)iter;//(iter);
		}
		return NULL;
	}
public:
	multiset<UserInfo> items;
};
template<typename Container>
void eraseall(Container& con)
{
	Container::iterator iter = con.begin();
	while(!con.empty()/*iter != con.end()*/)
	{
		iter = con.erase(iter);//or
		//con.erase(iter++);
	}
}
int main ()
{
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vector<int>::iterator iter = vec.begin();
	//while(iter!=vec.end())
	//	iter = vec.erase(iter);

	list<int> li;
	li.push_back(1);
	li.push_back(2);
	li.push_back(3);
	list<int>::iterator iter2 = li.begin();
	//while(iter2!=li.end())
	//	iter2 = li.erase(iter2);
	map<int,int> mm;
	mm[2]=3;
	mm[2342]=3234;
	mm[23]=343;
	map<int,int>::iterator iter3 = mm.begin();
	//while(iter3!=mm.end())
	//	iter3 = mm.erase(iter3);

	eraseall(vec);
	eraseall(li);
	eraseall(mm);

	TopList tl;
	UserInfo u1("name1",13);
	tl.RefreshTopList(u1);
	UserInfo u2("name2",36);
	tl.RefreshTopList(u2);
	UserInfo u3("name3",7);
	tl.RefreshTopList(u3);
	UserInfo u4("name4",25);
	tl.RefreshTopList(u4);

	int score = tl.GetRankByName(string("name4"));
	UserInfo *pu = tl.GetUserByRank(7);
	pu->score = 100;

	cout<<sizeof(twt)<<endl;
	test_Construction();
	test_Count();
	test_equal_ranges();
	test_erase();
	test_insert();

	test_set_algorithm();

	system("pause");
	return 0;
} 