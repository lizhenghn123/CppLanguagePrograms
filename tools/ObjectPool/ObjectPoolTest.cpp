#include <iostream>
#include <string>
#include "ObjectPool.h"
#include "base/Singleton.h"
#include "thread/FastMutex.h"
using namespace std;

class TO
{
    char c;
    int i;
    float t;
    double d;
    string s;
};

class TO1
{
    char c;
    int i;
    float t;
    double d;
    string s;
    TO *to;
};


int main()
{
    {
        ObjectPool<TO, zl::thread::RecursiveMutex> pool;
        TO *t1 = pool.alloc();
        pool.free(t1);
        cout << t1 << "\n";
        TO *t2 = pool.alloc();
        cout << t2 << "\n";
        TO *t3 = pool.alloc();
        int t = t3 - t2;
        cout << t2 << "\t" << t3 << "\t" << t3 - t2 << "\t" << sizeof(TO) << "\n";
        cout << "---------------\n";
    }
    {
        ObjectPool<TO1, zl::thread::NullMutex> pool;   
        TO1 *t1 = pool.alloc();          
        TO1 *t2 = pool.alloc();           
        cout << t1 << "\t" << t2 << "\t" << t2 - t1 << "\t" << sizeof(TO1) << "\n";  
        cout << "---------------\n";
    }
    {
        ObjectPool<TO1, zl::thread::FastMutex> pool;   
        TO1 *t1 = pool.alloc();          
        TO1 *t2 = pool.alloc();           
        cout << t1 << "\t" << t2 << "\t" << t2 - t1 << "\t" << sizeof(TO1) << "\n";  
        cout << "---------------\n";
    }
    system("pause");
}