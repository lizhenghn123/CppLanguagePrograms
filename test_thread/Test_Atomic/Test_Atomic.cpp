#include <iostream>
#include "thread/Atomic.h"
using namespace std;


int main()
{
	{
		zl::thread::Atomic ato;
		ato++;
		++ato;
		ato.FetchAndInc(10);
		ato += 100;
		ato -= 3;
		std::cout << ato.Value() << "\n";

		long t = ato.DecAndFetch(3);
		std::cout << ato << "\t" << t << "\n";
	}
	{
		std::cout << "========================================\n";
		zl::thread::Atomic ato;

		std::cout << ato.FetchAndInc(2) << "\t" ;
		std::cout << ato.Value() << "\n";

		std::cout << ato.FetchAndDec(3) << "\t" ;
		std::cout << ato.Value() << "\n";
	}
	{
		std::cout << "========================================\n";
		zl::thread::Atomic ato;

		std::cout << ato.IncAndFetch(2) << "\t" ;
		std::cout << ato << "\n";
	}
	system("pause");
	return 0;
}