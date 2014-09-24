#include <iostream>
#include "base/ProcessTimeCounter.h"
using namespace std;

void test()
{
	ProcessTimeCounter pc;
	pc.Start();
	long long sum = 1;
	for (int i = 1; i < 20; ++i)
	{
		sum *= i;
		std::cout << sum << "\n";
	}
	cout << sum << "\n";

	for (int i = 1; i < 200; ++i)
		cout << "hello world ";
	pc.Stop();

	cout << "---------------------\n";

	std::cout << "KernelPeriodCount : " << pc.KernelPeriodCount() << std::endl;
	std::cout << "KernelSeconds     : " << pc.KernelSeconds() << std::endl;
	std::cout << "KernelMillSeconds : " << pc.KernelMillSeconds() << std::endl;
	std::cout << "KernelMicroseconds: " << pc.KernelMicroseconds() << std::endl;

	std::cout << "UserPeriodCount   : " << pc.UserPeriodCount() << std::endl;
	std::cout << "UserSeconds       : " << pc.UserSeconds() << std::endl;
	std::cout << "UserMillSeconds   : " << pc.UserMillSeconds() << std::endl;
	std::cout << "UserMicroSeconds  : " << pc.UserMicroSeconds() << std::endl;

	std::cout << "PeriodCount       : " << pc.PeriodCount() << std::endl;
	std::cout << "Seconds           : " << pc.Seconds() << std::endl;
	std::cout << "MillSeconds       : " << pc.MillSeconds() << std::endl;
	std::cout << "MicroSeconds      : " << pc.MicroSeconds() << std::endl;
}
int main()
{
	test();
	system("pause");
	return 0;
}