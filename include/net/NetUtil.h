// ***********************************************************************
// Filename         : NetUtil.h
// Author           : LIZHENG
// Created          : 2014-09-17
// Description      : 网络相关工具类
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-09-17
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_NETUTIL_H
#define ZL_NETUTIL_H
#include "Define.h"
NAMESPACE_ZL_NET_START

class NetUtil
{
public:
    /**
    * @brief           判断传入的IP字符串是否是广播地址
    * @param  str      IP地址字符串
    * @return          如果str为广播地址，则返回true，否则返回false
    */
    static bool        IsBroadcastAddress(const char *str);

    /**
    * @brief           判断给定的字符串是否为一个IPv4或IPv6地址
    * @param  str      IP地址字符串
    * @return          如果是一个IP地址，则返回true，否则返回false
    */
    static bool        IsValidIp(const char *str);

    /**
    * @brief           判断给定的字符串是否为一个IPv4地址
    * @param  str      IP地址字符串
    * @return          如果是一个IPv4地址，则返回true，否则返回false
    */
    static bool        IsValidIpv4(const char *str);

    /**
    * @brief           判断给定的字符串是否为一个IPv6地址
    * @param  str      IP地址字符串
    * @return          如果是一个IPv6地址，则返回true，否则返回false
    */
    static bool        IsValidIpv6(const char *str);

    /**
    * @brief           CPU大小端，字节序判断
    * @return          如果是小端字节序，如果是返回true，否则返回false
    */
    static bool        IsLittleEndian();

    /**
    * @brief           反转字节序，以便在网络上传输
    * @param  source   源字节
    * @param  result   反转后的字节
    * @param  length   反转的长度
    */
    static void        ReverseBytes(const void *source, void *result, size_t length);

    /**
    * @brief           反转字节序，以便在网络上传输
    * @param  source   源字节
    * @return          反转后的字节
    */
    template <typename DataType>
    static DataType    ReverseBytes(const DataType& source)
    {
        DataType result = 0;
        ReverseBytes(&source, &result, sizeof(result));
        return result;
    }

    /**
    * @brief           反转字节序，以便在网络上传输
    * @param  source   源字节
    * @param  result   反转后的字节
    */
    template <typename DataType>
    static void        ReverseBytes(const DataType *source, DataType *result)
    {
        ReverseBytes(source, result, sizeof(DataType));
    }

    /**
    * @brief           将源数据从主机字节序转换成网络字节序
    * @param  source   主机字节序源数据
    * @param  result   存放转换后的网络字节序结果数据
    * @param  length   需要转换的字节长度
    */
    static void        Host2Net(const void *source, void *result, size_t length);

    /**
    * @brief           将源数据从主机字节序转换成网络字节序
    * @param  source   主机字节序源数据
    * @param  result   存放转换后的网络字节序结果数据
    */
    template <typename DataType>
    static void        Host2Net(const DataType& source, DataType& result)
    {
        Host2Net(&source, &result, sizeof(result));
    }

    /**
    * @brief           将源数据从主机字节序转换成网络字节序
    * @param  source   主机字节序源数据
    * @return          存放转换后的网络字节序结果数据
    */
    template <typename DataType>
    static DataType    Host2Net(const DataType& source)
    {
        DataType result;
        Host2Net(&source, &result, sizeof(result));
        return result;
    }

    /**
    * @brief           将源数据从网络字节序转换成主机字节序
    * @param  source   网络字节序源数据
    * @param  result   存放转换后的主机字节序结果数据
    * @param  length   需要转换的字节长度
    */
    static void        Net2Host(const void *source, void *result, size_t length);

    /**
    * @brief           将源数据从网络字节序转换成主机字节序
    * @param  source   网络字节序源数据
    * @param  result   存放转换后的主机字节序结果数据
    */
    template <typename DataType>
    static void        Net2Host(const DataType& source, DataType& result)
    {
        Host2Net<DataType>(source, result);
    }

    /**
    * @brief           将源数据从网络字节序转换成主机字节序
    * @param  source   网络字节序源数据
    * @return          转换后的主机字节序结果数据
    */
    template <typename DataType>
    static DataType Net2Host(const DataType& source)
    {
        return Host2Net<DataType>(source);
    }
};

NAMESPACE_ZL_NET_END
#endif  /* ZL_NETUTIL_H */