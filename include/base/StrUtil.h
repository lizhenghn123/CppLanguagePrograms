// ***********************************************************************
// Filename         : StrUtil.h
// Author           : LIZHENG
// Created          : 2014-08-16
// Description      : 字符串辅助函数
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-10-14
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_STRUTIL_H
#define ZL_STRUTIL_H
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>

namespace zl
{
    /** 任意类型转为字符串 */
    template <typename T>
    inline std::string ToStr(const T& t)
    {
        std::ostringstream oss;
        oss << t;
        return oss.str();
    }

    /** 字符串转为某一类型 */
    template <typename T>
    T StrTo(const std::string& str)
    {
        T t;
        std::ostringstream iss(str);
        iss >> t;
        return t;
    }

    /** 将字符串转为小写并返回 */
    inline std::string ToLower(const std::string& str)
    {
        std::string t = str;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        return t;
    }

    /** 将字符串转为小写并返回 */
    inline std::string ToUpper(const std::string& str)
    {
        std::string t = str;
        std::transform(t.begin(), t.end(), t.begin(), ::toupper);
        return t;
    }

    /** 去掉字符串中左边的空格 */
    inline void TrimLeft(std::string& str)
    {
        str.erase(0, str.find_first_not_of(' '));
    }

    /** 去掉字符串中右边的空格 */
    inline void TrimRight(std::string& str)
    {
        str.erase(str.find_last_not_of(' ') + 1);
    }

    /** 去掉字符串中两端的空格 */
    inline void Trim(std::string& str)
    {
        TrimLeft(str);
        TrimRight(str);
    }

    /** 去掉字符串中的所有空格 */
    inline void TrimAll(std::string& str)
    {
        str.erase(std::remove_if(str.begin(), str.end(),
                                 std::bind2nd(std::equal_to<char>(), ' ')), str.end());
    }

    /** 去掉字符串中的特定字符 */
    inline void Erase(std::string& str, const char& charactor)
    {
        str.erase(std::remove_if(str.begin(), str.end(),
                                 std::bind2nd(std::equal_to<char>(), charactor)), str.end());
    }

    /** 判断字符串是否以某一子串为开始 */
    inline bool	StartWith(const std::string& str, const std::string& substr)
    {
        return str.find(substr) == 0;
    }

    /** 判断字符串是否以某一子串为结尾 */
    inline bool EndWith(const std::string& str, const std::string& substr)
    {
        return str.rfind(substr) == (str.length() - substr.length());
    }

    /** 比较两个字符串是否相等（忽略大小写） */
    inline bool equalsIgnoreCase(const std::string& lhs, const std::string& rhs)
    {
        return ToLower(lhs) == ToLower(rhs);
    }

    /** 字符串替换 */
    inline void Replace(std::string& strSrc, const std::string& strDeliter, const std::string& strDest)
    {
        while(true)
        {
            size_t pos = strSrc.find(strDeliter);

            if(pos != std::string::npos)
                strSrc.replace(pos, strDeliter.size(), strDest);
            else
                break;
        }
    }

    /** 字符串分隔，如果有连续的delim，是否插入空串 */
    inline void Split(const std::string& str, std::vector<std::string>& result,
                      const std::string& delim = " ", bool insertEmpty = false)
    {
        if(str.empty() || delim.empty())
            return;

        std::string::const_iterator substart = str.begin(), subend;
        while(true)
        {
            subend = std::search(substart, str.end(), delim.begin(), delim.end());
            std::string temp(substart, subend);

            if(!temp.empty())
            {
                if(insertEmpty)
                    result.push_back("");
                result.push_back(temp);
            }
            if(subend == str.end())
                break;

            substart = subend + delim.size();
        }
    }

} /* namespace zl */

#endif  /* ZL_STRUTIL_H */