#include <iostream>
#include "thread/Atomic.h"
using namespace std;


int main()
{
	{
		ZL::Atomic ato;
		ato++;
		++ato;
		ato.IncAndFetch(10);
		ato += 100;
		ato -= 3;
		std::cout << ato << "\n";
	}
	system("pause");
	return 0;
}