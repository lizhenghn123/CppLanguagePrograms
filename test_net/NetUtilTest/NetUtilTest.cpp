#include <iostream>
#include "net/NetUtil.h"
using namespace std;

int main()
{
	printf("is little endian: %d\n",   zl::net::NetUtil::isLittleEndian());
	printf("is valid ipaddress: %d\n", zl::net::NetUtil::isValidIpv4("1.2.3.4"));
	printf("is valid ipaddress: %d\n", zl::net::NetUtil::isValidIpv4("1x.2.3.4"));
	printf("is valid ipaddress: %d\n", zl::net::NetUtil::isValidIpv4("11711.2.3.4"));
	printf("is valid ipaddress: %d\n", zl::net::NetUtil::isValidIpv4("11.2.3.4345"));
	return 0;
}
