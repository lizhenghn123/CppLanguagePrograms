#include <iostream>
#include "HttpRequest.h"
#include <assert.h>
using namespace std;

int main()
{
    {
        HttpRequest req(false, false);
        req.Get("http://www.baidu.com");
        assert(req.GetHeader().empty());
        assert(req.GetBody().empty());
        system("pause");
    }
    {
        HttpRequest req(true, false);
        req.Get("http://www.baidu.com");
        cout << req.GetHeader() << "\n";
        assert(req.GetBody().empty());
        system("pause");
    }
    {
        HttpRequest req(false, true);
        req.Get("http://www.baidu.com");
        assert(req.GetHeader().empty());
        cout << req.GetBody() << "\n";
        system("pause");
    }
    {
        HttpRequest req(true, true);
        req.AddHeader("User-Agent", "Mozilla/4.04[en](Win95;I;Nav)");
        req.Get("https://www.baidu.com");
        cout << req.GetHeader() << "\n";
        cout << req.GetBody() << "\n";
        system("pause");
    }

    cout << "###### GAME OVER ######\n";
    system("pause");
}