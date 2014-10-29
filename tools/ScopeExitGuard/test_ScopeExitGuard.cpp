#include <iostream>
#include <Windows.h>
#include <assert.h>
#include "base/ScopeExitGuard.h"
using namespace std;
using namespace zl::base;

template < typename T >
struct FClose
{
	void operator()(T handle)
	{
		int ret = fclose(handle);
		cout << ret << "\n";
	}
};


int main()
{

	{
		FILE *file = fopen("f.log", "w");
		//ScopeExitGuard<FILE*, FClose, 0> ff(file);
		//ScopeExitGuard<FILE*, detail::DeletePolicy, 0> ff(file);
		ScopeCloseFile ff(file);
	    fwrite("123", 1, 3, file);
		//auto_file ff(file);
	}
	{
		int *p = new int;
		//ScopeExitGuard<int*, detail::DeletePolicy, 0> ff(p);
		ScopeExitGuard<int*> fqf(p);
		//ScopeDeleteMem ff(p);
		*p = 5;
	}
	return 0;
}