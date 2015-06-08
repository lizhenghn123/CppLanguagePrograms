// Filename         : HttpRequest.h
// Author           : LIZHENG
// Created          : 2013-05-12
// Description      : 对libcurl的包装，对外提供GET、POST接口
//
// Last Modified By : LIZHENG
// Last Modified On : 2015-06-04
//
// Copyright (c) lizheng@thinkit.cn. All rights reserved.
// ***********************************************************************
#ifndef ZL_HTTPREQUEST_H
#define ZL_HTTPREQUEST_H
#include <vector>
#include <string>
struct curl_slist;

class HttpRequest
{
public:
    struct Reponse
    {
        std::string header_;
        std::string body_;
    };

public:
    HttpRequest();
    ~HttpRequest();

public:
    /**
    * @brief 设置是否调试输出
    * @param bDebug    输入参数,为true则输出调试信息
    */
    static void SetDebug(bool bDebug)
    {
        debug_ = bDebug;
    }

    static bool IsDebug()
    {
        return debug_;
    }

    /**
    * @brief 获取错误码对应的错误消息
    * @param errcode   输入参数,curl错误码（参见curl.h中CURLcode定义）
    * @return          如果错误码errcode为curl标准错误码，则返回有效字符串，否则为NULL
    */
    static const char* GetErrorMessage(int errcode);

public:
    /**
    * @brief 返回Http响应的消息头
    */
    std::string  GetHeader() const { return reponse_.header_; }

    /**
    * @brief 返回Http响应的消息体
    */
    std::string  GetBody() const   { return reponse_.body_; }

public:
    /**
    * @brief 设置http请求时的头
    * @param key         输入参数,key
    * @param value       输入参数,value
    */
    void AddHeader(const char *key, const char *value);

    /**
    * @brief HTTP GET请求
    * @param url         输入参数,请求的url地址
    * @param timeoutMs   输入参数，本次请求的超时时间,单位毫秒，下同
    * @return            返回标识,只有为CURLE_OK(0)时表示成功，其他都是失败
    */
    int Get(const char *url, int timeoutMs = 4000);

    /**
    * @brief HTTP POST请求
    * @param url         输入参数,请求的url地址
    * @param postData    输入参数，需要post的数据
    * @param dataSize    输入参数,需要post的数据大小
    * @param strResponse 输出参数,返回的内容
    * @return            返回标识,只有为CURLE_OK(0)时表示成功，其他都是失败
    */
    int Post(const char *url, const char *postData, int dataSize, int timeoutMs = 4000);

    /**
    * @brief HTTP POST请求
    * @param url         输入参数,请求的url地址
    * @param postData    输入参数，需要post的数据
    * @param strResponse 输出参数,返回的内容
    * @return            返回标识,只有为CURLE_OK(0)时表示成功，其他都是失败
    */
    int Post(const char *url, const std::string& postData, int timeoutMs = 4000);

    /**
    * @brief HTTPS GET请求,证书版本
    * @param url         输入参数,请求的url地址
    * @param strResponse 输出参数,返回的内容
    * @param pCaPath     输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
    * @return            返回标识,只有为CURLE_OK(0)时表示成功，其他都是失败
    */
    int Gets(const char *url, const char *pCaPath = NULL, int timeoutMs = 4000);

    /**
    * @brief HTTPS POST请求,证书版本
    * @param url         输入参数,请求的url地址
    * @param strPost     输入参数,需要post的数据
    * @param strResponse 输出参数,返回的内容
    * @param pCaPath     输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
    * @return            返回标识,只有为CURLE_OK(0)时表示成功，其他都是失败
    */
    int Posts(const char *url, const std::string& postData, const char *pCaPath = NULL, int timeoutMs = 4000);

private:
    static bool          debug_;       /**是否开启调试输出*/

private:
    struct curl_slist    *headers_;
    //CURL                 *curl;
    Reponse              reponse_;
};

#endif  /* ZL_HTTPREQUEST_H */