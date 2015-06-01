#include "GBKtoUTF8.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <string.h>
#include <memory.h>
#include <assert.h>


#ifdef OS_WINDOWS
#include <Windows.h>
#elif defined(OS_LINUX)
#include <iconv.h>
#else
#error "You must be include OsDefine.h firstly"
#endif

NAMESPACE_ZL_BASE_START

#ifdef OS_WINDOWS

std::string ConvertGBKToUtf8(const std::string& strGBK )
{
    int len = MultiByteToWideChar( CP_ACP, 0, (LPCTSTR)strGBK.c_str(), -1, NULL, 0 );
    unsigned short *wszUtf8 = new unsigned short[len + 1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar( CP_ACP, 0, (LPCTSTR)strGBK.c_str(), -1, (LPWSTR)wszUtf8, len );
    len = WideCharToMultiByte( CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL );
    char *szUtf8 = new char[len + 1];
    memset( szUtf8, 0, len + 1 );
    WideCharToMultiByte ( CP_UTF8, 0, (LPWSTR)wszUtf8, -1, (LPSTR)szUtf8, len, NULL, NULL );
    std::string strUtf8 = szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;
    return strUtf8;
}

std::string ConvertGBKToUtf8(const char *strGBK, int len)
{
    std::string gbk(strGBK, len);
    return ConvertGBKToUtf8(gbk);
}

std::string ConvertUtf8ToGBK(const std::string& strUtf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, NULL, 0);
    unsigned short *wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL, NULL);
    std::string strGBK = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
    return strGBK;
}

std::string ConvertUtf8ToGBK(const char *strUtf8, int len)
{
    std::string utf8(strUtf8, len);
    return ConvertUtf8ToGBK(utf8);
}

#elif defined(OS_LINUX)

bool code_convert(const char *from_charset, const char *to_charset, char *inbuf, size_t inlen , char *outbuf, size_t outlen);

std::string ConvertGBKToUtf8(const char *strGBK, int len)
{
    char *cname = new char[len + 1];
    memset(cname, '\0', len + 1);
    memcpy(cname, strGBK, len);
    char *cdst = new char[len * 3];
    memset(cdst, '\0', len * 3);
    bool code = code_convert("gbk", "utf-8", cname, len, cdst, len * 3);
    std::string strUtf8;
    if (code)
    {
        strUtf8 = cdst;
    }
    delete[] cname;
    delete[] cdst;
    return strUtf8;
}

std::string ConvertGBKToUtf8(const std::string& strGBK)
{
    return ConvertGBKToUtf8(strGBK.c_str(), strGBK.size());
}

std::string ConvertUtf8ToGBK(const char *strUtf8, int len)
{
    char *cname = new char[len + 1];
    memset(cname, '\0', len + 1);
    memcpy(cname, strUtf8, len);
    char *cdst = new char[len + 1];
    memset(cdst, '\0', len + 1);
    bool code = code_convert("utf-8", "gbk", cname, len, cdst, len);
    std::string strGBK;
    if (code)
    {
        strGBK = cdst;
    }
    delete[] cname;
    delete[] cdst;
    return strGBK;
}

std::string ConvertUtf8ToGBK(const std::string& strUtf8)
{
    return ConvertUtf8ToGBK(strUtf8.c_str(), strUtf8.size());
}

bool code_convert(const char *from_charset, const char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    iconv_t cd ;
    char **pin = &inbuf ;
    char **pout = &outbuf ;
    cd = iconv_open( to_charset , from_charset );
    if( cd == 0 )
    {
        return false;
    }
    memset( outbuf , 0 , outlen );
    int convert = iconv( cd , pin , &inlen , pout , &outlen );
    if( convert == -1 )
    {
        iconv_close( cd );
        return false ;
    }
    iconv_close(cd);
    return true ;
}
#endif

/*下面两个IsTextUTF8都无法检测出gb2312编码的汉字“谢谢”*/
bool IsTextUTF8(const char *str, int length)
{
#ifndef OS_WINDOWS
    typedef unsigned long DWORD;
    typedef unsigned char UCHAR;
#endif
    int i;
    DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    UCHAR chr;
    bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
    for(i = 0; i < length; i++)
    {
        chr = (UCHAR) * (str + i);
        if( (chr & 0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
            bAllAscii = false;
        if(nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
        {
            if(chr >= 0x80)
            {
                if(chr >= 0xFC && chr <= 0xFD)
                    nBytes = 6;
                else if(chr >= 0xF8)
                    nBytes = 5;
                else if(chr >= 0xF0)
                    nBytes = 4;
                else if(chr >= 0xE0)
                    nBytes = 3;
                else if(chr >= 0xC0)
                    nBytes = 2;
                else
                {
                    return false;
                }
                nBytes--;
            }
        }
        else //多字节符的非首字节,应为 10xxxxxx
        {
            if((chr & 0xC0) != 0x80 )
            {
                return false;
            }
            nBytes--;
        }
    }
    if( nBytes > 0 ) //违返规则
    {
        return false;
    }
    if(bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
    {
        return false;
    }
    return true;
}

int is_utf8_special_byte(unsigned char c)
{
    unsigned special_byte = 0X02; //binary 00000010
    if (c >> 6 == special_byte)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool IsTextUTF8(const std::string& str)
{
    unsigned one_byte   = 0X00;    //binary 00000000
    unsigned two_byte   = 0X06;    //binary 00000110
    unsigned three_byte = 0X0E;    //binary 00001110
    unsigned four_byte  = 0X1E;    //binary 00011110
    unsigned five_byte  = 0X3E;    //binary 00111110
    unsigned six_byte   = 0X7E;    //binary 01111110
    unsigned char k = 0;
    unsigned char m = 0;
    unsigned char n = 0;
    unsigned char p = 0;
    unsigned char q = 0;
    unsigned char c = 0;
    bool	isUtf8 = false;
    for (size_t i = 0; i < str.size();)
    {
        c = (unsigned char)str[i];
        if (c >> 7 == one_byte)
        {
            i++;
            continue;
        }
        else if (c >> 5 == two_byte)
        {
            k = (unsigned char)str[i + 1];
            if ( is_utf8_special_byte(k) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 2;
                continue;
            }
        }
        else if (c >> 4 == three_byte)
        {
            m = (unsigned char)str[i + 1];
            n = (unsigned char)str[i + 2];
            if ( is_utf8_special_byte(m) && is_utf8_special_byte(n) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 3;
                continue;
            }
        }
        else if (c >> 3 == four_byte)
        {
            k = (unsigned char)str[i + 1];
            m = (unsigned char)str[i + 2];
            n = (unsigned char)str[i + 3];
            if ( is_utf8_special_byte(k)
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 4;
                continue;
            }
        }
        else if (c >> 2 == five_byte)
        {
            unsigned char k = (unsigned char)str[i + 1];
            unsigned char m = (unsigned char)str[i + 2];
            unsigned char n = (unsigned char)str[i + 3];
            unsigned char p = (unsigned char)str[i + 4];
            if ( is_utf8_special_byte(k)
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n)
                    && is_utf8_special_byte(p) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 5;
                continue;
            }
        }
        else if (c >> 1 == six_byte)
        {
            k = (unsigned char)str[i + 1];
            m = (unsigned char)str[i + 2];
            n = (unsigned char)str[i + 3];
            p = (unsigned char)str[i + 4];
            q = (unsigned char)str[i + 5];
            if ( is_utf8_special_byte(k)
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n)
                    && is_utf8_special_byte(p)
                    && is_utf8_special_byte(q) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 6;
                continue;
            }
        }
        if(!isUtf8)
            return false;
        else
            i++;
    }
    return true;
}

NAMESPACE_ZL_BASE_END