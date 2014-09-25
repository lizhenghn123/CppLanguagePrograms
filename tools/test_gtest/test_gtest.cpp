#include <iostream>
#include "gtest/gtest.h"
using namespace std;

int GCD(int a, int b)  //最大公约数
{
	if (a == 0 || b == 0)
	{
		throw "don't do that";
	}
	int c = a % b;
	if (c == 0)
		return b;
	return GCD(b, c);
}

TEST(GCDTest1, CaclGCD1)
{
	EXPECT_EQ(5, GCD(4, 10));
	EXPECT_EQ(6, GCD(30, 18));
}

TEST(GCDTest2, CaclGCD2)
{
	ASSERT_EQ(5, GCD(4, 10));
	ASSERT_EQ(6, GCD(30, 18));
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}