#ifndef GBK_TO_UNICODE_FILE_H
#define GBK_TO_UNICODE_FILE_H

#include <string>

/**检测字符串是否是utf8编码*/
bool IsTextUTF8(const char* str, int length);
bool IsTextUTF8(const std::string& str);

/**GBK和UTF-8的编码转换*/
std::string ConvertGBKToUtf8(const std::string& strGBK);
std::string ConvertUtf8ToGBK(const std::string& strUtf8);


#endif