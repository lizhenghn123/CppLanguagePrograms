#include <iostream>
#include <ostream>
#include <iterator>
#include <set>
#include <algorithm>
using namespace std;

void print (set<int, less<int> >& s)
{
	set<int, less<int> >::iterator It;
	for ( It = s.begin(); It != s.end(); It++ )
		cout << *It << " ";
	cout << endl;
}
void test_Construction()
{
	int ary[] = { 5,3,7,5,2,3,7,5,5,4 };
	set<int> s1;
	set<int, greater<int> > s2;

	for ( int i=0; i<sizeof(ary)/sizeof(int); i++ )
	{
		s1.insert(ary[i]);
		s2.insert(ary[i]);
	}
	cout << "s1 : ";
	copy(s1.begin(),s1.end(),ostream_iterator<int>(cout," "));
	cout<<endl;
	set<int>::iterator iter1 = s1.begin();
	cout << "s1 : ";
	while ( iter1 != s1.end() )
		cout << *(iter1++) << " ";
	cout << endl;
	set<int, greater<int> >::iterator iter2 = s2.begin();
	cout << "s2 : ";
	while ( iter2 != s2.end() )
		cout << *(iter2++) << " ";
	cout << endl;

	// second form of constructor
	set<int> s3(ary,ary+3);
	set<int>::iterator iter3 = s3.begin();
	cout << "s3 : ";
	while ( iter3 != s3.end() )
		cout << *(iter3++) << " ";
	cout << endl;

	// copy constructor (predicate of s1 is important)
	set<int, less<int> > s4(s1);
	set<int, less<int> >::iterator iter4 = s4.begin();
	cout << "s4 : ";
	while ( iter4 != s4.end() )
		cout << *(iter4++) << " ";
	cout << endl;
};
void test_Count()
{
	int ary[] = {1,2,3,2,3,4,8,2,5,6};
	set<int, less<int> > s;
	s.insert(ary,ary+10);

	cout << "s : ";
	copy(s.begin(),s.end(),ostream_iterator<int>(cout," "));
	cout<<endl;

	int n = s.count(2);
	cout << "count of '2' (0 or 1) is " << n << endl;
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
	set<int, less<int> > s;

	s.insert(ary,ary+10);

	s.erase(2); // erase '2'

	set<int, less<int> >::iterator It;

	It = s.find(5);
	s.erase(It); // erase '5'

	It = s.find(4);
	s.erase(It,s.end()); // erase from It to the end of set
}
void test_insert()
{
	int ary[] = {1,2,3,2,3,4,8,2,5,6};
	set<int, less<int> > s;

	s.insert(10);
	print(s);

	s.insert(ary,ary+5);
	print(s);

	set<int, less<int> >::iterator It = s.begin();
	s.insert(It,20);
	print(s);
}

template <typename T>
void truefalse(T t)
{
	cout << (t?"True":"False") << endl;
}
void test_key_comp()
{
	set<int> s;

	cout << "s.key_comp()(1,2) returned ";
	truefalse(s.key_comp()(1,2));  // True

	cout << "s.key_comp()(2,1) returned ";
	truefalse(s.key_comp()(2,1));  // False

	cout << "s.key_comp()(1,1) returned ";
	truefalse(s.key_comp()(1,1));  // False
}
void test_value_comp()
{
	set<int> s;

	cout << "s.value_comp()(1,2) returned ";
	truefalse(s.value_comp()(1,2));  // True

	cout << "s.value_comp()(2,1) returned ";
	truefalse(s.value_comp()(2,1));  // False

	cout << "s.value_comp()(1,1) returned ";
	truefalse(s.value_comp()(1,1));  // False
}
//void test_erase()
//{
//
//}
typedef struct  lodinfo
{
	lodinfo(int i=0,int l=0):id(i),lod(l){}
	lodinfo(const lodinfo& p){
		id= p.id;
		lod=p.lod;
	}
	int id;
	int lod;
}lodinfo;
bool operator< (const lodinfo& lod1,const lodinfo& lod2)
{
	//return lod1.id < lod2.id;
	return (lod1.id==lod2.id) ? (lod1.lod < lod2.lod) : (lod1.id < lod2.id);
}
bool operator== (const lodinfo& lod1,const lodinfo& lod2)
{
	return lod1.id==lod2.id;
}
class set_lodinfo_equal : public unary_function<lodinfo,bool>
{
	lodinfo lod;
	//pair<lodinfo,int> lll;
public:
	explicit set_lodinfo_equal(const lodinfo& ll) : lod(ll) {	}
	//explicit set_lodinfo_equal(const pair<lodinfo,int>& ll) : lll(ll) {	}
	bool operator()(const lodinfo& _p)
	{
		return lod.id == _p.id;//(lll->first).id == _p.id;
	}
};
int main ()
{
	set<lodinfo> sss;
	sss.insert(lodinfo(1,34));
	sss.insert(lodinfo(2,12));
	//sss.insert(lodinfo(1,5));
	lodinfo lod1(1,5),lod2(3,567);
	set<lodinfo>::iterator iter1 = find_if(sss.begin(),sss.end(),set_lodinfo_equal(lod1));
	if(iter1==sss.end())
		sss.insert(lod1);
	set<lodinfo>::iterator iter2 = find_if(sss.begin(),sss.end(),set_lodinfo_equal(lod2));
	if(iter2==sss.end())
		sss.insert(lod2);

	test_Construction();
	test_Count();
	test_equal_ranges();
	test_erase();
	test_insert();
	test_key_comp();
	test_value_comp();

	system("pause");
	return 0;
} 