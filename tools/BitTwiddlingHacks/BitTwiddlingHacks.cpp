// come from http://graphics.stanford.edu/~seander/bithacks.html
#include <iostream>
#include <string>
using namespace std;

//Detect if two integers have opposite signs
bool DetectIfOppoSign(int x, int y)
{
    bool f = ((x ^ y) < 0); // true if x and y have opposite signs
    return f;
}

// count the number of bits set in v, 统计二进制中1个个数
int GetBitNum(unsigned int v)
{
    int c; // c accumulates the total bits set in v
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    //for (c = 0; v; c++)  //ok
    //{
    //	v &= v - 1; // clear the least significant bit set
    //}
    return c;
}

// 计算v之后的下一个数，且该数 == 2^n, 即 求m，m=2^n && m >= v
int NextPowerOf2(int v)
{
    int c = 0;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    c = ++v;
    return c;
}

int main()
{
    {
        std::string tts_text("快递查询格式:快递公司名@订单号例:ems@EG893082275CS");
        const static std::string jinhao_key("@");
        const static std::string maohao_key(":");
        size_t pos = tts_text.find(jinhao_key);
        while(pos != std::string::npos)
        {
            tts_text.replace(pos, 1, "");
            pos = tts_text.find(jinhao_key);
        }
        pos = tts_text.find(maohao_key);
        while(pos != std::string::npos)
        {
            tts_text.replace(pos, 1, "");
            pos = tts_text.find(maohao_key);
        }
        std::cout << tts_text << "\n";
    }
    std::cout << DetectIfOppoSign(1, 3) << "\n";   // 0
    std::cout << DetectIfOppoSign(1, -3) << "\n";  // 1
    std::cout << DetectIfOppoSign(1, 0) << "\n";   // 0
    std::cout << DetectIfOppoSign(-1, 0) << "\n";  // 1
    std::cout << DetectIfOppoSign(-1, -2) << "\n"; // 0
    //////// Compute the integer absolute value (abs) without branching ///////////
    int v = -100;           // we want to find the absolute value of v
    unsigned int r;  // the result goes here
    int const mask = v >> sizeof(int) * CHAR_BIT - 1;
    r = (v + mask) ^ mask;
    std::cout << r << "\t" << mask << "\n";
    std::cout << "GetBitNum: " << GetBitNum(12345) << "\n";
    std::cout << "GetBitNum: " << GetBitNum(356) << "\n";
    std::cout << "NextPowerOf2: " << NextPowerOf2(12) << "\n"; // 16
    std::cout << "NextPowerOf2: " << NextPowerOf2(32) << "\n"; // 32
    {
    }
    system("pause");
    return 0;
}