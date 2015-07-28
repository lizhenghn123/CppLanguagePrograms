#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <iterator>
#include <algorithm>
#include <assert.h>
#include "base/GBKtoUTF8.h"
#include "GBKtoUTF8.inl"
using namespace std;
using namespace zl::base;

/**计算字符串的二进制表示*/
std::string PrintStringAsBinaryString(const char *str);
std::string PrintStringAsBinaryString(const std::string& str);

/**Unicode和UTF-8的编码转换*/
unsigned short one_gbk_to_unicode(unsigned char ch, unsigned char cl);
unsigned short one_unicode_to_gbk(unsigned short unicode); //not implement
void unicode_to_gbk(unsigned short *punicode, char *pgbk, int len);
int one_unicode_to_utf8(unsigned long unic, unsigned char *pOutput, int outSize);

//参数1是UTF8字符串当前位置指针，这里必须要是指针，因为必须要通过第1个字符进行判断才知道一个完整的字符的编码要向后取多少个字符
//参数2是返回的UCS-2编码的Unicode字符
int one_utf8_to_unicode(const char *utf8, unsigned short& wch);
//参数1是UTF8编码的字符串
//参数2是输出的UCS-2的Unicode字符串
//参数3是参数1字符串的长度
//使用的时候需要注意参数2所指向的内存块足够用。其实安全的办法是判断一下pUniBuf是否为NULL，如果为NULL则只统计输出长度不写pUniBuf，这样
//通过两次函数调用就可以计算出实际所需要的Unicode缓存输出长度。当然，更简单的思路是：无论如何转换，UTF8的字符数量不可能比Unicode少，所
//以可以简单的按照sizeof(wchar_t) * utf8Leng来分配pUniBuf的内存
int utf8_to_unicode(const char *utf8Buf, unsigned short *pUniBuf, int utf8Leng);

int unicode_to_utf8(unsigned short wchar, char *utf8);

void GetText(bool isUtf8, std::vector<std::string>& vecStr)
{
    const char *filename = isUtf8 ? "utf8.txt" : "gbk.txt";
    std::ifstream ifs(filename);
    assert(!ifs.fail());
    while(!ifs.eof())
    {
        std::string str;
        ifs >> str;
        vecStr.push_back(str);
    }
    ifs.close();
}

void test_gbbk_utf_unicode()
{
    char const *p = "你";
    std::cout << PrintStringAsBinaryString(p) << "\n";
    std::cout << PrintStringAsBinaryString("位") << "\n";
    //将bgbk按空格分开，每连续两个表示是一个字符的二进制编码，转成int类型后通过one_gbk_to_unicode调用，得到该字符的unicode编码，然后通过one_unicode_to_utf8转为utf-8
    //一的二进制表示是11010010 10111011（D2BB，210-187），通过one_gbk_to_unicode转换为unicode是4E00，然后通过one_unicode_to_utf8转为utf-8
    std::cout << std::hex << one_gbk_to_unicode(196, 227) << "\n";
    std::cout << std::hex << one_gbk_to_unicode(186, 195) << "\n";
    std::cout << std::hex << one_gbk_to_unicode(210, 187) << "\n";
    std::cout << std::hex << one_gbk_to_unicode(182, 254) << "\n";
    unsigned short uni = 0x4E00;
    char pgbk[32];
    unicode_to_gbk(&uni, pgbk, 32);
    std::cout << pgbk << "\n";
    unsigned long one = 0x4E00; //0xD2BB;//0x4E00;
    unsigned char pOutput[6];
    memset(pOutput, 0, 6);
    one_unicode_to_utf8(one, pOutput, 6);
    cout << pOutput << "\n";
    std::cout << PrintStringAsBinaryString((char *)pOutput) << "\n";
}

void test1()
{
    std::cout << PrintStringAsBinaryString("你") << "\n";
    std::cout << PrintStringAsBinaryString("好") << "\n";
    std::cout << PrintStringAsBinaryString("你好") << "\n";
    const char *p = "浣犲ソ";//"涓�"; //utf-8字符
    unsigned short unibuf[32];
    utf8_to_unicode(p, unibuf, 32);
    std::cout << unibuf << "--" << PrintStringAsBinaryString((char *)unibuf) << "\n";
    unsigned short wch = 0;
    one_utf8_to_unicode(p, wch);
    std::cout << wch << "--" << PrintStringAsBinaryString((char *)&wch) << "\n";
    unsigned short unicode = 0x4E00;
    char buf[32] = {'0'};
    unicode_to_utf8(unicode, buf);
}

struct set_key_comp : public binary_function<std::string, std::string, bool>
{
    bool operator()(const std::string& _Left, const std::string& _Right) const
    {
        //在这里实现你的任意排序方式
        return _Right < _Left;
        return _Left < _Right;
    }
};

void test_set()
{
    std::string s1 = "赵薇";
    std::string s2 = "黄晓明";
    std::string s3 = "华X";
    std::string s4 = "黄Z";
    std::string s5 = "阿莎";
    std::set<std::string> ss1;
    std::set<std::string, set_key_comp> ss2;
    std::cout << PrintStringAsBinaryString(s1) << "\t" << s1 << "\n";
    std::cout << PrintStringAsBinaryString(s2) << "\t" << s2 << "\n";
    std::cout << PrintStringAsBinaryString(s3) << "\t" << s3 << "\n";
    std::cout << PrintStringAsBinaryString(s4) << "\t" << s4 << "\n";
    std::cout << PrintStringAsBinaryString(s5) << "\t" << s5 << "\n";
    ss1.insert(s1);
    ss1.insert(s2);
    ss1.insert(s3);
    ss1.insert(s4);
    ss1.insert(s5);
    ss2.insert(s1);
    ss2.insert(s2);
    ss2.insert(s3);
    ss2.insert(s4);
    ss2.insert(s5);
    for(std::set<std::string>::iterator itr = ss1.begin(); itr != ss1.end(); ++itr)
        std::cout << *itr << "\t";
    std::cout << "\n";
    for(std::set<std::string, set_key_comp>::iterator itr = ss2.begin(); itr != ss2.end(); ++itr)
        std::cout << *itr << "\t";
    std::cout << "\n";
    //std::copy(ss1.begin(),ss2.end(),std::back_inserter(std::cout));
    //std::copy(ss1.begin(), ss1.end(), std::back_inserter(ss2));
}

size_t get_utf8_length(const std::string& input)
{
    size_t length = 0;
    for (size_t i = 0, len = 0; i != input.length(); i += len)
    {
        unsigned char byte = input[i];
        if (byte >= 0xFC) // lenght 6
            len = 6;
        else if (byte >= 0xF8)
            len = 5;
        else if (byte >= 0xF0)
            len = 4;
        else if (byte >= 0xE0)
            len = 3;
        else if (byte >= 0xC0)
            len = 2;
        else
            len = 1;
        length ++;
    }
    return length;
}

int main()
{
    std::cout << get_utf8_length("谢谢") << "\n";
    std::cout << get_utf8_length("谢 谢!") << "\n";
    test_set();
    system("pause");
    return 0;
    //test1();
    //std::cout<<"-----------------------------------------\n";
    //test_gbbk_utf_unicode();
    //std::cout<<"-----------------------------------------\n";
    std::cout << "字: GBK: " << PrintStringAsBinaryString("字") << "\n"; //11010111 11010110 : 215 214
    std::cout << "字: Unicode: " << std::hex << one_gbk_to_unicode(215, 214) << "\n"; //5b57
    unsigned short unicode = 0x5b57;
    char buf[32] = {'0'};
    unicode_to_utf8(unicode, buf);
    std::cout << "字: Utf-8: " << PrintStringAsBinaryString(buf) << "\t" << buf << "\n"; //11100101 10101101 10010111 : E5 AD 97
    std::cout << "-----------------------------------------\n";
    std::string ghello = "你好";
    std::string gthank = "谢谢";
    std::cout << ghello << "\t" << convertGBKToUtf8(ghello) << "\t" << isTextUTF8(ghello.c_str(), ghello.size()) << "\t" << isTextUTF8(ghello) << "\n";
    std::cout << gthank << "\t" << convertGBKToUtf8(gthank) << "\t" << isTextUTF8(gthank.c_str(), gthank.size()) << "\t" << isTextUTF8(gthank) << "\n";
    std::vector<std::string> uVecStr;
    std::vector<std::string> gVecStr;
    GetText(true, uVecStr);
    GetText(false, gVecStr);
    std::cout << "--------------utf8--->gbk----------------\n";
    for (std::vector<std::string>::iterator itr = uVecStr.begin(); itr != uVecStr.end(); ++itr)
    {
        std::cout << *itr << "\t" << convertUtf8ToGBK(*itr) << "\t" << isTextUTF8((*itr).c_str(), (*itr).size()) << "\t" << isTextUTF8(*itr) << "\n";
    }
    std::cout << "--------------gbk--->utf8----------------\n";
    for (std::vector<std::string>::iterator itr = gVecStr.begin(); itr != gVecStr.end(); ++itr)
    {
        std::cout << *itr << "\t" << convertGBKToUtf8(*itr) << "\t" << isTextUTF8((*itr).c_str(), (*itr).size()) << "\t" << isTextUTF8(*itr) << "\n";
    }
    std::cout << "---------------------------------\n";
    system("pause");
    return 0;
}


/*****************************************************************************
* 该代码来自：http://blog.csdn.net/csfreebird/article/details/8571726
* 输出字符串的二进制格式
****************************************************************************/
// Get the bit value specified by the index,index starts with 0
template<class T>
int Bit_Value(T value, int index)
{
    return (value & (1 << index)) == 0 ? 0 : 1;
}

// T must be one of integer type
template<class T>
std::string PrintIntAsBinaryString(T v)
{
    std::stringstream stream;
    int i = sizeof(T) * 8 - 1;
    while (i >= 0)
    {
        stream << Bit_Value(v, i);
        --i;
    }
    return stream.str();
}

/**计算字符串的二进制表示*/
std::string PrintStringAsBinaryString(const char *p)
{
    std::stringstream stream;
    for (size_t i = 0; i < strlen(p); ++i)
    {
        stream << PrintIntAsBinaryString(p[i]);
        stream << " ";
    }
    return stream.str();
}

/**计算字符串的二进制表示*/
std::string PrintStringAsBinaryString(const std::string& str)
{
    std::stringstream stream;
    for (size_t i = 0; i < str.size(); ++i)
    {
        stream << PrintIntAsBinaryString(str[i]);
        stream << " ";
    }
    return stream.str();
}

extern const unsigned short const gbk_to_unicode_table[];
extern const unicode_to_gbk_t const unicode_to_gbk_table[];
#define U2W_LOBYTE(w)           ((unsigned char)(((unsigned short)(w)) & 0xff))
#define U2W_HIBYTE(w)           ((unsigned char)((((unsigned short)(w)) >> 8) & 0xff))

unsigned short one_gbk_to_unicode(unsigned char ch, unsigned char cl)
{
    ch -= 0x81;
    cl -= 0x40;
    return (ch <= 0x7d && cl <= 0xbe) ? gbk_to_unicode_table[ch * 0xbf + cl] : 0x1fff;
}

unsigned short one_unicode_to_gbk(unsigned short unicode)
{
    const static int  TABLE_SIZE = (sizeof(unicode_to_gbk_table) / sizeof(unicode_to_gbk_table[0]));
    int i, b, e;
    b = 0;
    e = TABLE_SIZE - 1;
    while (b <= e)
    {
        i = (b + e) / 2;
        if (unicode_to_gbk_table[i].unicode == unicode)
            return unicode_to_gbk_table[i].gbk;
        if (unicode_to_gbk_table[i].unicode < unicode)
            b = i + 1;
        else
            e = i - 1;
    }
    return 0;
}

void unicode_to_gbk(unsigned short *punicode, char *pgbk, int len)
{
    while (*punicode && len > 0)
    {
        unsigned short dbcs;
        dbcs = one_unicode_to_gbk(*punicode);
        if (dbcs > 0x0ff || dbcs < 0)
        {
            *pgbk = dbcs >> 8;
            ++pgbk;
        }
        *pgbk = U2W_LOBYTE(dbcs);
        ++pgbk;
        --len;
        ++punicode;
    }
    *pgbk = 0x00;
}

/*****************************************************************************
 * 该代码来自：http://blog.csdn.net/tge7618291/article/details/7608510
 * 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
 *
 * 参数:
 *    unic     字符的Unicode编码值
 *    pOutput  指向输出的用于存储UTF8编码值的缓冲区的指针
 *    outsize  pOutput缓冲的大小
 *
 * 返回值:
 *    返回转换后的字符的UTF8编码所占的字节数, 如果出错则返回 0 .
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!
 ****************************************************************************/
int one_unicode_to_utf8(unsigned long unic, unsigned char *pOutput, int outSize)
{
    assert(pOutput != NULL);
    assert(outSize >= 6);
    if ( unic <= 0x0000007F )
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        *pOutput     = (unic & 0x7F);
        return 1;
    }
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        *(pOutput + 1) = (unic & 0x3F) | 0x80;
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput + 2) = (unic & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >>  6) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 3) = (unic & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 4) = (unic & 0x3F) | 0x80;
        *(pOutput + 3) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 5) = (unic & 0x3F) | 0x80;
        *(pOutput + 4) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput + 3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }
    return 0;
}

/*****************************************************************************
 * 该代码来自：http://blog.csdn.net/coollofty/article/details/8058859
 * 参数1是UTF8字符串当前位置指针，这里必须要是指针，因为必须要通过第1个字符进行判断才知道一个完整的字符的编码要向后取多少个字符
 * 参数2是返回的UCS-2编码的Unicode字符
 ****************************************************************************/
int one_utf8_to_unicode(const char *utf8, unsigned short& wch)
{
    //首字符的Ascii码大于0xC0才需要向后判断，否则，就肯定是单个ANSI字符了
    unsigned char firstCh = utf8[0];
    if (firstCh >= 0xC0)
    {
        //根据首字符的高位判断这是几个字母的UTF8编码
        int afters, code;
        if ((firstCh & 0xE0) == 0xC0)
        {
            afters = 2;
            code = firstCh & 0x1F;
        }
        else if ((firstCh & 0xF0) == 0xE0)
        {
            afters = 3;
            code = firstCh & 0xF;
        }
        else if ((firstCh & 0xF8) == 0xF0)
        {
            afters = 4;
            code = firstCh & 0x7;
        }
        else if ((firstCh & 0xFC) == 0xF8)
        {
            afters = 5;
            code = firstCh & 0x3;
        }
        else if ((firstCh & 0xFE) == 0xFC)
        {
            afters = 6;
            code = firstCh & 0x1;
        }
        else
        {
            wch = firstCh;
            return 1;
        }
        //知道了字节数量之后，还需要向后检查一下，如果检查失败，就简单的认为此UTF8编码有问题，或者不是UTF8编码，于是当成一个ANSI来返回处理
        for(int k = 1; k < afters; ++ k)
        {
            if ((utf8[k] & 0xC0) != 0x80)
            {
                //判断失败，不符合UTF8编码的规则，直接当成一个ANSI字符返回
                wch = firstCh;
                return 1;
            }
            code <<= 6;
            code |= (unsigned char)utf8[k] & 0x3F;
        }
        wch = code;
        return afters;
    }
    else
    {
        wch = firstCh;
    }
    return 1;
}

/*****************************************************************************
 * 参数1是UTF8编码的字符串
 * 参数2是输出的UCS-2的Unicode字符串
 * 参数3是参数1字符串的长度
 * 使用的时候需要注意参数2所指向的内存块足够用。其实安全的办法是判断一下pUniBuf是否为NULL，如果为NULL则只统计输出长度不写pUniBuf，这样
 * 通过两次函数调用就可以计算出实际所需要的Unicode缓存输出长度。当然，更简单的思路是：无论如何转换，UTF8的字符数量不可能比Unicode少，所
 * 以可以简单的按照sizeof(wchar_t) * utf8Leng来分配pUniBuf的内存……
 ****************************************************************************/
int utf8_to_unicode(const char *utf8Buf, unsigned short *pUniBuf, int utf8Leng)
{
    int i = 0, count = 0;
    while(i < utf8Leng)
    {
        i += one_utf8_to_unicode(utf8Buf + i, pUniBuf[count]);
        count ++;
    }
    return count;
}

int unicode_to_utf8(unsigned short wchar, char *utf8)
{
    int len = 0;
    if (wchar < 0xC0)
    {
        utf8[len ++] = (char)wchar;
    }
    else if (wchar < 0x800)
    {
        utf8[len ++] = 0xc0 | (wchar >> 6);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    else if (wchar < 0x10000)
    {
        utf8[len ++] = 0xe0 | (wchar >> 12);
        utf8[len ++] = 0x80 | ((wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    else if (wchar < 0x200000)
    {
        utf8[len ++] = 0xf0 | ((int)wchar >> 18);
        utf8[len ++] = 0x80 | ((wchar >> 12) & 0x3f);
        utf8[len ++] = 0x80 | ((wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    else if (wchar < 0x4000000)
    {
        utf8[len ++] = 0xf8 | ((int)wchar >> 24);
        utf8[len ++] = 0x80 | ((wchar >> 18) & 0x3f);
        utf8[len ++] = 0x80 | ((wchar >> 12) & 0x3f);
        utf8[len ++] = 0x80 | ((wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    else if (wchar < 0x80000000)
    {
        utf8[len ++] = 0xfc | ((int)wchar >> 30);
        utf8[len ++] = 0x80 | ((wchar >> 24) & 0x3f);
        utf8[len ++] = 0x80 | ((wchar >> 18) & 0x3f);
        utf8[len ++] = 0x80 | ((wchar >> 12) & 0x3f);
        utf8[len ++] = 0x80 | ((wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    return len;
}