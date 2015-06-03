#include <iostream>
#include <string>
#include <stdint.h>
#include <assert.h>
#include "utility/SHA1.h"
using namespace std;
using namespace zl::utility;

int main()
{
	assert(SHA1::HexDigest("") == std::string("da39a3ee5e6b4b0d3255bfef95601890afd80709"));

	assert(SHA1::HexDigest("hello world") == std::string("2aae6c35c94fcfb415dbe95f408b9ce91ee846ed"));
	assert(SHA1::HexDigest("hello+world") == std::string("66506355462a1a5653b6c40e1be6df1146a84c21"));
	
	assert(SHA1::HexDigest("∑≤»ÀΩ‘”–“ªÀ¿") == std::string("fab4112f102622bc3653079884433b9588ca5e06"));

	system("pause");
}
