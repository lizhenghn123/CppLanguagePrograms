#include <iostream>
#include <Windows.h>
#include <assert.h>
#include "base/ScopeExitGuard.h"
using namespace std;
using namespace zl::base;

class test
{
public:
    test() : res(0)
    {
        cout << "test(), now value is " << res << "\n";
    }
    ~test()
    {
        cout << "~test(), now value is " << res << "\n";
    }

    void reset()
    {
        res = 0;
    }

    bool process(bool b)
    {
        if(b)
        {
            res += 1;
            cout << "test::process ok\n";
        }
        else
        {
            res = -1;
            cout << "test::process fail\n";
        }
        return b;
    }

private:
    int res ;
};

void safe_delete(test * t)
{
    delete t; t = 0;
}

void test_scopeexitguard()
{
    {
        FILE *file = fopen("f.log", "w");
        ScopeExitGuard seg([&] { fclose(file); });
        //ScopeExitGuard seg2(std::bind(fclose, std::placeholders::_1));
        //std::function<int ()> func =std::bind(fclose, file); 
        fwrite("123", 1, 3, file);
    }
    {
        int *p = new int;
        ScopeExitGuard seg([&] { delete p; p = NULL; });
        *p = 5;
    }
    {
        int *p = new int;
        ON_SCOPE_EXIT([&] { delete p; p = NULL; });
        *p = 5;
    }
    {
        test *p = new test;
        //ON_SCOPE_EXIT([&] { delete p; p = NULL; });
        ScopeExitGuard seg(std::bind(safe_delete, p));
        p->process(true);
        throw;
    }
}

void test_rollback()
{
    cout << "===========rollback 1===========\n";
    while(1)
    {
        test t;
        ScopeExitGuard onFailureRollback([&] { t.reset(); });
        if(t.process(false) == false) // fail
            break;
        onFailureRollback.dismiss();
        break;
    }
    cout << "===========rollback 2===========\n";
    while(1)
    {
        test t;
        ScopeExitGuard onFailureRollback([&] { t.reset(); });
        if(t.process(true) == false) // fail
            break;
        onFailureRollback.dismiss();
        break;
    }
}

int main()
{
    test_rollback();

    test_scopeexitguard();

    return 0;
}