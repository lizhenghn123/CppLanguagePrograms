#include <iostream>
#include "HttpRequest.h"
#include <assert.h>
#include "base/GBKtoUTF8.h"
using namespace std;

int main()
{
    {
        std::string s = "ÄãºÃ";
        s = zl::base::ConvertUtf8ToGBK(s);
        std::string url("http://192.168.14.208:80/tts.php?data=");
        url += s;
        url += "&speed=5&decode=6";
        cout << url << "\n";
        HttpRequest req;
        req.Get(url.c_str());
        cout << req.GetHeader() << "\n\n";
        cout << req.GetBody() << "\n\n";
        system("pause");
    }
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