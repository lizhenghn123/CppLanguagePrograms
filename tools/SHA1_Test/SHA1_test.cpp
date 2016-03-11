#include <iostream>
#include <string>
#include <stdint.h>
#include <assert.h>
#include "utility/SHA1.h"
#include "utility/Base64.h"
using namespace std;
using namespace zl::util;

#define all_length  60    // 24 + 36 sizeof(client.key) + sizeof(magic)
#define magic_len   36    // sizeof(magic)

const char* const kMagic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

int test_sha1(const char* clientkey)
{
    std::cout << clientkey << ", len = " << strlen(clientkey) << "\n";

    SHA1 sha;
    std::string key = string(clientkey) + kMagic;

    uint8_t buf[20];
    sha.update(key);
    sha.final(buf);

    key = base64Encode((const char*)buf, 20);


    std::cout << "accept key = " << key << ", size = " << key.size() << "\n\n";
    return 0;
}

int main()
{
    assert(SHA1::hexDigest("") == std::string("da39a3ee5e6b4b0d3255bfef95601890afd80709"));

    assert(SHA1::hexDigest("hello world") == std::string("2aae6c35c94fcfb415dbe95f408b9ce91ee846ed"));
    assert(SHA1::hexDigest("hello+world") == std::string("66506355462a1a5653b6c40e1be6df1146a84c21"));

    assert(SHA1::hexDigest("·²ÈË½ÔÓÐÒ»ËÀ") == std::string("fab4112f102622bc3653079884433b9588ca5e06"));

    test_sha1("EOg8TsF2zejaZxHQ2DCxMg==");  // e2MP56HaiHEIIISvXYIO0na5++8=
    test_sha1("5gP/kQ0Nu3+rnExy9nWzhA==");  // fxLDp3p6O+eZ+oUmW6unUt0SJxQ=
    test_sha1("P7qLIgwK9X/qYOUuc1vSPA==");  // jHUTZaE52JSrw0rWn09toszNZiY=
    system("pause");
}
