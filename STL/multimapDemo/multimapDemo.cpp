#include <iostream>
#include <istream>
#include <fstream>
#include <cstdio>
#include <map>
#include <string>
using namespace std;
class Nocase		//忽略大小写的比较
{
public:
	bool operator()(const string& x,const string& y)const
	{
		string::const_iterator p = x.begin();
		string::const_iterator q = y.begin();

		while (p!=x.end() && q!=y.end() && toupper(*p)== toupper(*q) )
		{
			++p;
			++q;
		}

		if(p==x.end())
			return q!=y.end();
		if(q==y.end())
			return false;
		return toupper(*p) < toupper(*q);
	}
};
void print (multimap<string, int, less<string> > &m)
{
	multimap<string, int, less<string> >::iterator It = m.begin();
	cout << "multimap :" << endl;
	while ( It != m.end() )
	{
		cout << (*It).first << " - " << (*It).second << endl;
		It++;
	}
}

void test_Construction()
{
	typedef multimap<int, char, less<int> > M;
	M m1;

	m1.insert(M::value_type(2,'B'));
	m1.insert(M::value_type(3,'C'));
	m1.insert(M::value_type(1,'A'));
	m1.insert(M::value_type(1,'a'));

	M::iterator It = m1.begin();
	cout << endl << "m1:" << endl;
	while ( It != m1.end() )
	{
		cout << (*It).first << " - "
			<< (*It).second
			<< endl;
		It++;
	}

	// copy constructor
	M m2(m1);

	It = m2.begin();
	cout << endl << "m2:" << endl;
	while ( It != m2.end() )
	{
		cout << (*It).first << " - "
			<< (*It).second
			<< endl;
		It++;
	}

	M m3(m2.begin(),m2.end());
	It = m3.begin();
	cout << endl << "m3:" << endl;
	while ( It != m3.end() )
	{
		cout << (*It).first << " -"	<< (*It).second << endl;
		It++;
	}
}
void test_Count()
{
	typedef multimap<char,string> M1;
	typedef M1::value_type v_t1;
	M1 m1;
	typedef multimap<string,char,less<string> > M2;
	typedef M2::value_type v_t2;
	M2 m2;

	string word;
	int counter = 0;
	ifstream In("/usr/share/dict/words");
	if ( In.good() )
	{
		while(1)
		{
			getline(In,word);
			char ch = word.at(0);
			// file is sorted
			if ( ch != 'A' && ch != 'a' )
				break;
			else
			{
				// for conting of words
				m1.insert(v_t1(ch,word));
				// for upper-lower bound
				m2.insert(v_t2(word,ch));
			}
			counter++;
		}
		In.close();
	}
	cout << "System Dictionary consists " << counter
		<< " words,\nwith first letter 'a' or 'A'"
		<< endl;
	cout << m1.count('A') << " words start with 'A'"
		<< endl;
	cout << m1.count('a') << " words start with 'a'"
		<< endl;
	M2::iterator low = m2.lower_bound("Aba");
	M2::iterator upp = m2.upper_bound("Abe");
	cout << "Range of the words from 'Aba' to 'Abe':"
		<< endl;
	while ( low != upp )
	{
		cout << (*low).first << endl;
		low++;
	}
}

void test_erase()
{
	typedef multimap<string, int,less<string> /*Nocase*/ > M;
	typedef M::value_type v_t;

	M m;
	m.insert(v_t("AAA",1));
	m.insert(v_t("BBB",2));
	m.insert(v_t("CCC",3));
	m.insert(v_t("EEE",4));
	m.insert(v_t("CCC",5));
	m.insert(v_t("DDD",6));

	print(m);

	m.erase("BBB"); // remove element with key 'BBB'
	print(m);

	M::iterator It;
	It = m.find("DDD");
	m.erase(It); // remove element pointed by It
	print(m);

	It = m.find("CCC");

	m.erase(m.begin(),It); // remove the range of elements
	print(m);
}
int main ()
{
	test_Construction();
	test_Count();
	test_erase();

	system("pause");
	return 0;
} 