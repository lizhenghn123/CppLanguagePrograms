#include <iostream>
#include "DateTimeUtil.h"
using namespace std;

int main()
{
	{
		const char* str = "1998-12-23 12:34:56";
		const char *pos = str;
		int i = atoi(pos);
		cout << i << "\n";

		pos = pos + sizeof("YYYY");
		cout << atoi(pos) << "\n";

		pos = pos + sizeof("DD");
		cout << atoi(pos) << "\n";

		pos = pos + sizeof("HH");
		cout << atoi(pos) << "\n";

		pos = pos + sizeof("MM");
		cout << atoi(pos) << "\n";

		pos = pos + sizeof("SS");
		cout << atoi(pos) << "\n";

		cout << "=====" << sizeof("YYYY") << "\n";
		const char* sss = "YYYY";
		cout << sizeof(sss) << "\t" << sizeof(*sss) << "\n";
	}

	{
		cout << DateTimeUtil::CurrentDateTime() << "\n";
		cout << DateTimeUtil::CurrentDate() << "\n";
		cout << DateTimeUtil::CurrentTime() << "\n";
	}
	system("pause");
	return 0;
}