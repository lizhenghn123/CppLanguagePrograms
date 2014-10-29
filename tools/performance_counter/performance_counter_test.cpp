#include <iostream>
#include "base/ProcessTimeCounter.h"
using namespace std;

void test()
{
	ProcessTimeCounter pc;
	pc.start();
	long long sum = 1;
	for (int i = 1; i < 20; ++i)
	{
		sum *= i;
		std::cout << sum << "\n";
	}
	cout << sum << "\n";

	for (int i = 1; i < 200; ++i)
		cout << "hello world ";
	pc.stop();

	cout << "---------------------\n";

	std::cout << "kernelPeriodCount : " << pc.kernelPeriodCount() << std::endl;
	std::cout << "kernelSeconds     : " << pc.kernelSeconds() << std::endl;
	std::cout << "kernelMillSeconds : " << pc.kernelMillSeconds() << std::endl;
	std::cout << "kernelMicroseconds: " << pc.kernelMicroseconds() << std::endl;

	std::cout << "userPeriodCount   : " << pc.userPeriodCount() << std::endl;
	std::cout << "userSeconds       : " << pc.userSeconds() << std::endl;
	std::cout << "userMillSeconds   : " << pc.userMillSeconds() << std::endl;
	std::cout << "userMicroSeconds  : " << pc.userMicroSeconds() << std::endl;

	std::cout << "PeriodCount       : " << pc.periodCount() << std::endl;
	std::cout << "Seconds           : " << pc.seconds() << std::endl;
	std::cout << "MillSeconds       : " << pc.millSeconds() << std::endl;
	std::cout << "MicroSeconds      : " << pc.microSeconds() << std::endl;
}
int main()
{
	test();
	system("pause");
	return 0;
}