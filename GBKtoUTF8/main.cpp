#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string.h>
#include <memory.h>
#include <assert.h>

#include "GBKtoUTF8.h"
#include "gbk2unicode.h"

using namespace std;

void GetText(bool isUtf8, std::vector<std::string>& vecStr)
{
	const char* filename = isUtf8 ? "utf8.txt" : "gbk.txt";
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

/*****************************************************************************
* 该代码来自：http://blog.csdn.net/csfreebird/article/details/8571726
* 输出字符串的二进制格式
****************************************************************************/

// Get the bit value specified by the index  
// index starts with 0  
template<class T>  
int Bit_Value(T value, int index) {  
	return (value & (1 << index)) == 0 ? 0 : 1;  
}  

// T must be one of integer type  
template<class T>  
string PrintIntAsBinaryString(T v) {  
	stringstream stream;  
	int i = sizeof(T) * 8 - 1;  
	while (i >= 0) {  
		stream << Bit_Value(v, i);  
		--i;  
	}  

	return stream.str();  
}  

/**计算字符串的二进制表示*/
string PrintStringAsBinaryString(const char* p) {  
	stringstream stream;  
	for (size_t i = 0; i < strlen(p); ++i) {  
		stream << PrintIntAsBinaryString(p[i]);  
		stream << " ";  
	}  
	return stream.str();  
}  

/**计算字符串的二进制表示*/
string PrintStringAsBinaryString(const string& str) {  
	stringstream stream;  
	for (size_t i = 0; i < str.size(); ++i) {  
		stream << PrintIntAsBinaryString(str[i]);  
		stream << " ";  
	}  
	return stream.str();  
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
int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,
        int outSize)
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
        *(pOutput+1) = (unic & 0x3F) | 0x80;
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput+2) = (unic & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+3) = (unic & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+4) = (unic & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+5) = (unic & 0x3F) | 0x80;
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }

    return 0;
}

/*****************************************************************************
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码
 *    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                类型为unsigned long .
 *
 * 返回值:
 *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic)
{
    assert(pInput != NULL && Unic != NULL);

    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;

    *Unic = 0x0; // 把 *Unic 初始化为全零
    int utfbytes  = 100;//= enc_get_utf8_size(*pInput);
    unsigned char *pOutput = (unsigned char *) Unic;

    switch ( utfbytes )
    {
        case 0:
            *pOutput     = *pInput;
            utfbytes    += 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xE0) != 0x80 )
                return 0;
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                    || ((b6 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
            break;
    }

    return utfbytes;
}

void test_gbbk_utf_unicode()
{
	char const * p = "一二";
	std::cout<<PrintStringAsBinaryString(p)<<"\n";
	std::cout<<PrintStringAsBinaryString("位")<<"\n";

	//将bgbk按空格分开，每连续两个表示是一个字符的二进制编码，转成int类型后通过gbk2unicode调用，得到该字符的unicode编码，然后通过enc_unicode_to_utf8_one转为utf-8

	//一的二进制表示是11010010 10111011（D2BB，210-187），通过gbk2unicode转换为unicode是4E00，然后通过enc_unicode_to_utf8_one转为utf-8
	cout << std::hex<<gbk2unicode(210,187) <<"\n";
	cout << std::hex<<gbk2unicode(182,254) <<"\n";
	unsigned long one = 0x4E8C; //0xD2BB;//0x4E00;
	unsigned char pOutput[32]; //utf-8最长是4字节
	memset(pOutput,0,24);
	enc_unicode_to_utf8_one(one,pOutput,24);
	cout << pOutput << "\n";
}
int main()
{
	//test_gbbk_utf_unicode();
	
	std::string ghello = "你好";
	std::string gthank = "谢谢";

	std::vector<std::string> uVecStr;
	std::vector<std::string> gVecStr;
	GetText(true,uVecStr);
	GetText(false,gVecStr);

	std::cout<<ghello<<"\t"<<ConvertGBKToUtf8(ghello)<<"\t"<<IsTextUTF8(ghello.c_str(),ghello.size())<<"\t"<<IsTextUTF8(ghello)<<"\n";
	std::cout<<gthank<<"\t"<<ConvertGBKToUtf8(gthank)<<"\t"<<IsTextUTF8(gthank.c_str(),gthank.size())<<"\t"<<IsTextUTF8(gthank)<<"\n";

	std::cout<<"--------------utf8--->gbk----------------\n";
	for (std::vector<std::string>::iterator itr = uVecStr.begin();itr!=uVecStr.end();++itr)
	{
		std::cout<<*itr<<"\t"<<ConvertUtf8ToGBK(*itr)<<"\t"<<IsTextUTF8((*itr).c_str(),(*itr).size())<<"\t"<<IsTextUTF8(*itr)<<"\n";
	}

	std::cout<<"--------------gbk--->utf8----------------\n";
	for (std::vector<std::string>::iterator itr = gVecStr.begin();itr!=gVecStr.end();++itr)
	{
		std::cout<<*itr<<"\t"<<ConvertGBKToUtf8(*itr)<<"\t"<<IsTextUTF8((*itr).c_str(),(*itr).size())<<"\t"<<IsTextUTF8(*itr)<<"\n";
	}

	std::cout<<"---------------------------------\n";
	system("pause");
	return 0;
}