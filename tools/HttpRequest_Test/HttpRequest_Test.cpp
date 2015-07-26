#include <iostream>
#include "HttpRequest.h"
#include <assert.h>
#include "base/GBKtoUTF8.h"
using namespace std;

int main()
{
    {
        HttpRequest req(true, true);
        req.setUserAgent("MyTestClient");
        req.get("http://www.baidu.com", "1.html");
        cout << req.getHeader() << "\n";
        cout << req.getBody() << "\n";
        system("pause");
    }
    {
        HttpRequest req(false, false);
        req.get("http://www.baidu.com");
        assert(req.getHeader().empty());
        assert(req.getBody().empty());
        system("pause");
    }
    {
        HttpRequest req(true, false);
        req.get("http://www.baidu.com");
        cout << req.getHeader() << "\n";
        assert(req.getBody().empty());
        system("pause");
    }
    {
        HttpRequest req(false, true);
        req.get("http://www.baidu.com");
        assert(req.getHeader().empty());
        cout << req.getBody() << "\n";
        system("pause");
    }
    {
        HttpRequest req(true, true);
        req.addHeader("User-Agent", "Mozilla/4.04[en](Win95;I;Nav)");
        req.get("https://www.baidu.com");
        cout << req.getHeader() << "\n";
        cout << req.getBody() << "\n";
        system("pause");
    }

    cout << "###### GAME OVER ######\n";
    system("pause");
}