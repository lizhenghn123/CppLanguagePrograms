#include <iostream>
#include "thread/Atomic.h"
using namespace std;


int main()
{
	{
		zl::thread::Atomic ato;
		ato++;
		++ato;
		ato.fetchAndInc(10);
		ato += 100;
		ato -= 3;
		std::cout << ato.value() << "\n";

		long t = ato.decAndFetch(3);
		std::cout << ato << "\t" << t << "\n";
	}
	{
		std::cout << "========================================\n";
		zl::thread::Atomic ato;

		std::cout << ato.fetchAndInc(2) << "\t" ;
		std::cout << ato.value() << "\n";

		std::cout << ato.fetchAndDec(3) << "\t" ;
		std::cout << ato.value() << "\n";
	}
	{
		std::cout << "========================================\n";
		zl::thread::Atomic ato;

		std::cout << ato.incAndFetch(2) << "\t" ;
		std::cout << ato << "\n";
	}
	system("pause");
	return 0;
}