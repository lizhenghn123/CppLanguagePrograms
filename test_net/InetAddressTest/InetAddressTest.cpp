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
        assert(addr0.IP() == std::string("0.0.0.0"));
        assert(addr0.Port() == 80);

        InetAddress addr1("192.22.12.78", 5000);
        assert(addr1.IP() == std::string("192.22.12.78"));
        assert(addr1.Port() == 5000);

        InetAddress addr2("255.254.253.252", 65535);
        assert(addr2.IP() == std::string("255.254.253.252"));
        assert(addr2.Port() == 65535);
    }
    {
        InetAddress addr0(80);
        bool res = InetAddress::Resolve("www.baidu.com", &addr0);
        cout << addr0.IP()<< "\t" << addr0.Port() << "\n";
    }
    system("pause");
    return 0;
}