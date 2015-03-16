#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <iterator>
#include <fstream>
#include "stl/iterator.h"
using namespace std;

void test_myiterator()
{
	int A[] = { 1, 2, 3, 4, 5, 6, 7 };
	std::cout << "distance of between A and A+7 = " << zl::stl::distance(A, A + 7) << std::endl;

	int *p = &A[0];
	zl::stl::advance(p, 3);
	std::cout << "*(p+3) = " << *p << std::endl;

	std::list<int> l;

//	std::copy(A, A + 7, zl::stl::front_inserter< std::list<int> >(l));
	std::copy(A, A + 7, zl::stl::back_inserter< std::list<int> >(l));
	std::copy(A, A + 7, std::back_inserter< std::list<int> >(l));
//	std::back_inserter 
//	std::copy(l.begin(), l.end(), zl::stl::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	zl::stl::reverse_iterator<int*> end(&A[0]);
	zl::stl::reverse_iterator<int*> begin(A + 7);
//	std::copy(begin, end, zl::stl::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	std::cout << "place input 10 numbers:" << std::endl;

	zl::stl::istream_iterator<int> ibegin(std::cin);
	zl::stl::istream_iterator<int> iend;
//	copy(ibegin, iend, l.begin());
//	std::copy(l.begin(), l.end(), zl::stl::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
}

int main()
{
	test_myiterator();

	{
		istream_iterator<int> cin_it(cin);
		istream_iterator<int> end_of_stream;
		
		vector<int> vec(cin_it, end_of_stream);   // ctrl + z 终止输入
		sort(vec.begin(), vec.end());
		
		ostream_iterator<int> output(cout, " ");
		unique_copy(vec.begin(), vec.end(), output);
		cout << endl;
	}
	istream_iterator<int> cin_it(cin);            //从cin读取int型数据
	istream_iterator<int> end_of_stream;            //指向超出末端位置

	ofstream outfile;
	ostream_iterator<int> output(outfile, " ");

	system("pause");
}