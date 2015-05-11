#include <iostream>
#include <assert.h>
#include <time.h>
#include <Windows.h>
#include "base/IntrusiveList.h"
using namespace std;

class MyClass : public Listable< MyClass >
{
public: 
    MyClass(int i=0)
    {
        id = i;
    }
    void print()
    {
        cout << "MyClass : " << id << "\n";
    }
private:
    int id;
};

int main()
{
    {
        ListQueue<MyClass> lq;
        for (int i = 0; i < 10; i++)
        {
            MyClass *pc = new MyClass(i);
            lq.push(pc);
        }
        cout << lq.size() << "\n";
        //cout << lq.empty() << "\n";

        lq.remove(lq.front());
        cout << lq.size() << "\n";

        MyClass *pc = new MyClass(100);
        lq.push(pc);
        cout << lq.size() << "\n";
    }
    {
        ListQueue<int> lq;
        for (int i = 0; i < 10; i++)
        {
            int *pc = new int(i);
            //lq.push(pc);  // not ok
        }
    }
    system("pause");
    return 0;
}