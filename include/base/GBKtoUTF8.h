// ***********************************************************************
// Filename         : GBKtoUTF8.h
// Author           : LIZHENG
// Created          : 2014-04-28
// Description      : GBK和Utf8的相互转换
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-04-30
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef GBK_TO_UTF8_FILE_H
#define GBK_TO_UTF8_FILE_H
#include "Define.h"
#include <string>
NAMESPACE_ZL_BASE_START

///   \brief    检测字符串是否是utf8编码
///   \param    str : 源字符串.
bool isTextUTF8(const std::string& str);

///   \brief    检测字符串是否是utf8编码
///   \param    str : 字符串
///   \param    len : 字符串长度
bool isTextUTF8(const char *str, int len);

///   \brief    转换utf8字符串为gbk字符串
///   \param    strGBK : 编码为gbk的字符串
///   \return   返回编码为utf8的字符串
std::string convertGBKToUtf8(const std::string& strGBK);

///   \brief    转换utf8字符串为gbk字符串
///   \param    strGBK : 编码为gbk的字符串
///   \param    len    : 源字符串长度
///   \return   返回编码为utf8的字符串
std::string convertGBKToUtf8(const char *strGBK, int len);

///   \brief    转换gbk字符串为utf8字符串
///   \param    strUtf8 : 编码为utf8的字符串
///   \return   返回编码为gbk的字符串
std::string convertUtf8ToGBK(const std::string& strUtf8);

///   \brief    转换gbk字符串为utf8字符串
///   \param    strUtf8 : 编码为utf8的字符串
///   \param    len     : 源字符串长度
///   \return   返回编码为gbk的字符串
std::string convertUtf8ToGBK(const char *strUtf8, int len);

NAMESPACE_ZL_BASE_END
#endif