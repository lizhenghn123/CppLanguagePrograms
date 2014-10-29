#include <iostream>
#include <assert.h>
#include "net/SocketUtil.h"
#include "net/InetAddress.h"
using namespace std;
using namespace zl;
using namespace zl::net;

int main()
{
    {
        InetAddress addr0(80);
        assert(addr0.ip() == std::string("0.0.0.0"));
        assert(addr0.port() == 80);

        InetAddress addr1("192.22.12.78", 5000);
        assert(addr1.ip() == std::string("192.22.12.78"));
        assert(addr1.port() == 5000);

        InetAddress addr2("255.254.253.252", 65535);
        assert(addr2.ip() == std::string("255.254.253.252"));
        assert(addr2.port() == 65535);
    }
    {
        InetAddress addr0(80);
        bool res = InetAddress::resolve("www.baidu.com", &addr0);
        cout << addr0.ip()<< "\t" << addr0.port() << "\n";
    }
    system("pause");
    return 0;
}