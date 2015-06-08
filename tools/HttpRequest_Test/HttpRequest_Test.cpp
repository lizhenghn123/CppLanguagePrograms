#include <iostream>
#include "HttpRequest.h"
using namespace std;

int main()
{
    {
        HttpRequest req;
        req.Get("http://www.baidu.com");
        //cout << req.GetHeader() << "\n";
        //cout << req.GetBody() << "\n";
    }
    {
        HttpRequest req;
        req.AddHeader("User-Agent", "Mozilla/4.04[en](Win95;I;Nav)");
        req.Get("https://www.baidu.com");
        cout << req.GetHeader() << "\n";
        //cout << req.GetBody() << "\n";
    }

    system("pause");
}