#include "HttpRestClient.h"
#include <curl/curl.h>
#include <assert.h>

#define JUST_RETUAN_IF_FALSE(expr, ret)  do { if(!expr) return ret;} while(0)
#define JUST_RETUAN_IF_TRUE(expr, ret)   do { if(expr) return ret;} while(0)
#define CURL_CHECK(func)                        \
            do {                                \
                CURLcode code = func;           \
                if(code != CURLE_OK)            \
                {                               \
                    fprintf(stderr, "CURL ERROR[%d][%s][%d]\n", code, __FILE__, __LINE__);  \
                    assert(code == CURLE_OK);   \
                }                               \
            }while(0)

namespace detail
{
    class CurlGlobalSettings
    {
    private:
        CurlGlobalSettings()
        {
            curl_global_init(CURL_GLOBAL_ALL);
        }
        ~CurlGlobalSettings()
        {
            curl_global_cleanup();
        }
    private:
        CurlGlobalSettings(const CurlGlobalSettings &);
        CurlGlobalSettings &operator=(const CurlGlobalSettings &);
    private:
        static CurlGlobalSettings curl_global;
    };

    CurlGlobalSettings CurlGlobalSettings::curl_global;

    static int debugCurlCallback(CURL*, curl_infotype itype, char* pData, size_t size, void*)
    {
        if (itype == CURLINFO_TEXT)
        {
            //printf("[TEXT]%s\n", pData);
        }
        else if (itype == CURLINFO_HEADER_IN)
        {
            printf("[HEADER_IN]%s\n", pData);
        }
        else if (itype == CURLINFO_HEADER_OUT)
        {
            printf("[HEADER_OUT]%s\n", pData);
        }
        else if (itype == CURLINFO_DATA_IN)
        {
            printf("[DATA_IN]%s\n", pData);
        }
        else if (itype == CURLINFO_DATA_OUT)
        {
            printf("[DATA_OUT]%s\n", pData);
        }
        return 0;
    }

    static CURL*  createCurl()
    {
        CURL *curl = curl_easy_init();
        if (HttpRestClient::isDebug())
        {
            CURL_CHECK(curl_easy_setopt(curl, CURLOPT_VERBOSE, 1));
            CURL_CHECK(curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debugCurlCallback));
        }
        return curl;
    }

    static void   setHttpsUrl(CURL* curl, const char* url)
    {
        if (strncmp(url, "https", 5) == 0)
        {
            CURL_CHECK(curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L));
            CURL_CHECK(curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L));
        }
    }

    static void   setCurlTimeout(CURL* curl, int timeoutMs)
    {
        if(timeoutMs <= 0)
            return;

        CURL_CHECK(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeoutMs));
        CURL_CHECK(curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs));
    }

    static int    performCurl(CURL* curl)
    {
        CURLcode code = curl_easy_perform(curl);
        if (code != CURLE_OK)
            return code;

        // 检查返回值
        long retCode = 200;
        code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retCode);
        if (code != CURLE_OK || retCode != 200)
        {
            fprintf(stderr, "curl_easy_getinfo = [%d][%ld]\n", code, retCode);
            return static_cast<int>(retCode);
        }
        return CURLE_OK;
    }

    static size_t writeHeaderCallback(char* buffer, size_t size, size_t memb, void* stream)
    {
        HttpRestClient::Reponse *rep = static_cast<HttpRestClient::Reponse*>(stream);

        size_t bodyLen = 0;
        int res = sscanf(buffer, "Content-Length:%lu", &bodyLen);
        if (res)
        {
            if (bodyLen > rep->body_.capacity())
                rep->body_.reserve(bodyLen);
        }

        size_t result = size * memb;
        rep->header_.append(buffer, buffer + result);
        return result;
    }

    static size_t writeBodyCallback(char* buffer, size_t size, size_t memb, void* stream)
    {
        HttpRestClient::Reponse *rep = static_cast<HttpRestClient::Reponse*>(stream);

        size_t result = size * memb;
        rep->body_.append(buffer, buffer + result);
        return result;
    }

    static size_t writeBodyToFileCallback(char* buffer, size_t size, size_t memb, void* stream)
    {
        FILE *file = static_cast<FILE *>(stream);
        size_t result = fwrite(buffer, size, memb,file);
        return result;
    }

    static size_t readDataCallback(char* buffer, size_t size, size_t memb, void* stream)
    {
        HttpRestClient::UploadObject *obj = static_cast<HttpRestClient::UploadObject*>(stream);

        size_t result = size * memb;
        size_t copy_size = (obj->length_ < result) ? obj->length_ : result;

        memcpy(buffer, obj->data_, copy_size);
        obj->length_ -= copy_size;
        obj->data_ += copy_size;

        return copy_size;
    }
}
using namespace detail;

bool HttpRestClient::debug_ = false;

HttpRestClient::HttpRestClient(bool requestHeader/* = true*/, bool requestBody/* = true*/)
{
    headers_ = NULL;
    headers_ = curl_slist_append(headers_, "Expect:");//也许有Expect: 100-continue，去掉它
    headers_ = curl_slist_append(headers_, "Accept:");//删除存在的消息头Accept
    //headers_ = curl_slist_append(headers_, "Host: 127.0.0.1:80"); //修改存在的消息头Host
    headers_ = curl_slist_append(headers_, "User-Agent: upload");
    headers_ = curl_slist_append(headers_, "Cache-Control: no-cache");
    headers_ = curl_slist_append(headers_, "Connection: Keep-Alive"); //http长连接

    curl_ = createCurl();

    // 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作
    // 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出
    CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_NOSIGNAL, 1));

    init(requestHeader, requestBody);
}

HttpRestClient::~HttpRestClient(void)
{
    if (headers_ != NULL)
    {
        curl_slist_free_all(headers_);
    }
    curl_easy_cleanup(curl_);
}

void HttpRestClient::init(bool requestHeader/* = true*/, bool requestBody/* = true*/)
{
    //assert(requestHeader || requestBody);
    if(requestHeader)
    {
        // 设置消息头的读取
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, writeHeaderCallback));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HEADERDATA, (void *)&reponse_));
    }
    else
    {
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, NULL));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HEADERDATA, NULL));
    }

    if(requestBody)
    {
        // 设置消息体的读取
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeBodyCallback));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void *)&reponse_));
    }
    else
    {
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, NULL));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_WRITEDATA, NULL));
    }
}

bool HttpRestClient::addHeader(const char* key, const char* value)
{
    std::string reqLine(key);
    reqLine += ": ";
    reqLine += value;
    headers_ = curl_slist_append(headers_, reqLine.c_str());
    return headers_ != NULL;
}

bool HttpRestClient::setUserAgent(const char* userAgent)
{
    assert(userAgent);
    int ret = curl_easy_setopt(curl_, CURLOPT_USERAGENT, userAgent);
    return ret == 0;
}

int HttpRestClient::get(const char* url, int timeoutMs/* = 4000*/)
{
    int ret = CURLE_OK;
    try
    {
        setHttpsUrl(curl_, url);

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_URL, url));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_READFUNCTION, NULL));
        //CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_));

        setCurlTimeout(curl_, timeoutMs);

        ret = performCurl(curl_);
    }
    catch (...)
    {
        return -1;
    }
    return ret;
}

int HttpRestClient::get(const char* url, const char* filepath, int timeoutMs/* = 4000*/)
{
    assert(url && filepath);
    FILE *file = fopen(filepath, "wb");
    if(!file) return -1;

    int ret = CURLE_OK;
    try
    {
        setHttpsUrl(curl_, url);

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_URL, url));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_READFUNCTION, NULL));
        //CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_));

        // 设置消息头的读取
        //CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, writeHeaderCallback));
        //CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HEADERDATA, (void *)&reponse_));

        // 设置消息体的读取
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeBodyToFileCallback));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_WRITEDATA, file));

        setCurlTimeout(curl_, timeoutMs);

        ret = performCurl(curl_);
    }
    catch (...)
    {
        fclose(file);
        return -1;
    }
    fclose(file);
    return ret;
}

int HttpRestClient::post(const char* url, const char* postData, int dataSize, int timeoutMs/* = 4000*/)
{
    int ret = CURLE_OK;
    try
    {
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_URL, url));

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_POST, 1));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, postData));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, dataSize));

        setCurlTimeout(curl_, timeoutMs);

        ret = performCurl(curl_);
    }
    catch (...)
    {
        return -1;
    }
    return ret;
}

int HttpRestClient::post(const char* url, const std::string& postData, int timeoutMs/* = 4000*/)
{
    return post(url, postData.c_str(), postData.size(), timeoutMs);
}

int HttpRestClient::del(const char* url, int timeoutMs/* = 4000*/)
{
    const static char *http_delete = "DELETE";
    int ret = CURLE_OK;
    try
    {
        setHttpsUrl(curl_, url);

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_URL, url));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_READFUNCTION, NULL));
        //CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_));

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, http_delete));

        setCurlTimeout(curl_, timeoutMs);

        ret = performCurl(curl_);
    }
    catch (...)
    {
        return -1;
    }
    return ret;
}

int HttpRestClient::put(const char* url, const char* putData, size_t dataSize)
{
    int ret = CURLE_OK;
    HttpRestClient::UploadObject obj = { putData, dataSize };
    try
    {
        setHttpsUrl(curl_, url);

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_URL, url));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_));

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_PUT, 1L));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_UPLOAD, 1L));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_READFUNCTION, readDataCallback));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_READDATA, &obj));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_INFILESIZE, static_cast<long>(obj.length_)));

        ret = performCurl(curl_);
    }
    catch (...)
    {
        return -1;
    }
    return ret;
}

int HttpRestClient::gets(const char* url, const char* pCaPath, int timeoutMs/* = 4000*/)
{
    int ret = CURLE_OK;
    try
    {
        setHttpsUrl(curl_, url);

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_URL, url));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_READFUNCTION, NULL));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_));

        if (NULL == pCaPath)
        {
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, false));
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, false));
        }
        else
        {
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, true));
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_CAINFO, pCaPath));
        }

        setCurlTimeout(curl_, timeoutMs);

        ret = performCurl(curl_);
    }
    catch (...)
    {
        return -1;
    }

    return ret;
}

int HttpRestClient::posts(const char* url, const std::string& strPost, const char* pCaPath, int timeoutMs/* = 4000*/)
{
    int ret = CURLE_OK;
    try
    {
        setHttpsUrl(curl_, url);

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_URL, url));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_));

        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_POST, 1));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, strPost.c_str()));
        CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, strPost.size()));

        if (NULL == pCaPath)
        {
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, false));
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, false));
        }
        else
        {
            //缺省情况就是PEM，所以无需设置，另外支持DER
            //curl_easy_setopt(curl_,CURLOPT_SSLCERTTYPE,"PEM");
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, true));
            CURL_CHECK(curl_easy_setopt(curl_, CURLOPT_CAINFO, pCaPath));
        }

        setCurlTimeout(curl_, timeoutMs);

        ret = performCurl(curl_);
    }
    catch (...)
    {
        return -1;
    }
    return ret;
}

const char* HttpRestClient::getErrorMessage(int errcode)
{
    //参考curl.h中的CURLcode定义
    static const int max_size_curl_error = 89;
    static const char *strCurlMessage[max_size_curl_error] =
    {
        "0(CURLE_OK)",
        "1(CURLE_UNSUPPORTED_PROTOCOL)",
        "2(CURLE_FAILED_INIT)",
        "3(CURLE_URL_MALFORMAT)",
        "4(CURLE_NOT_BUILT_IN)",
        "5(CURLE_COULDNT_RESOLVE_PROXY)",
        "6(CURLE_COULDNT_RESOLVE_HOST)",
        "7(CURLE_COULDNT_CONNECT)",
        "8(CURLE_FTP_WEIRD_SERVER_REPLY)",
        "9(CURLE_REMOTE_ACCESS_DENIED)",
        "10(CURLE_OBSOLETE10)",
        "11(CURLE_FTP_WEIRD_PASS_REPLY)",
        "12(CURLE_OBSOLETE12)",
        "13(CURLE_FTP_WEIRD_PASV_REPLY)",
        "14(CURLE_FTP_WEIRD_227_FORMAT)",
        "15(CURLE_FTP_CANT_GET_HOST)",
        "16(CURLE_OBSOLETE16)",
        "17(CURLE_FTP_COULDNT_SET_TYPE",
        "18(CURLE_PARTIAL_FILE",
        "19(CURLE_FTP_COULDNT_RETR_FILE)",
        "20(CURLE_OBSOLETE20)",
        "21(CURLE_QUOTE_ERROR",
        "22(CURLE_HTTP_RETURNED_ERROR",
        "23(CURLE_WRITE_ERROR)",
        "24(CURLE_OBSOLETE24)",
        "25(CURLE_UPLOAD_FAILED)",
        "26(CURLE_READ_ERROR)",
        "27(CURLE_OUT_OF_MEMORY)",
        "28(CURLE_OPERATION_TIMEDOUT)",
        "29(CURLE_OBSOLETE29)",
        "30(CURLE_FTP_PORT_FAILED)",
        "31(CURLE_FTP_COULDNT_USE_REST)",
        "32(CURLE_OBSOLETE32)",
        "33(CURLE_RANGE_ERROR)",
        "34(CURLE_HTTP_POST_ERROR)",
        "35(CURLE_SSL_CONNECT_ERROR)",
        "36(CURLE_BAD_DOWNLOAD_RESUME)",
        "37(CURLE_FILE_COULDNT_READ_FILE)",
        "38(CURLE_LDAP_CANNOT_BIND)",
        "39(CURLE_LDAP_SEARCH_FAILED)",
        "40(CURLE_OBSOLETE40)",
        "41(CURLE_FUNCTION_NOT_FOUND)",
        "42(CURLE_ABORTED_BY_CALLBACK)",
        "43(CURLE_BAD_FUNCTION_ARGUMENT)",
        "44(CURLE_OBSOLETE44)",
        "45(CURLE_INTERFACE_FAILED)",
        "46(CURLE_OBSOLETE46)",
        "47(CURLE_TOO_MANY_REDIRECTS)",
        "48(CURLE_UNKNOWN_OPTION)",
        "49(CURLE_TELNET_OPTION_SYNTAX)",
        "50(CURLE_OBSOLETE50)",
        "51(CURLE_PEER_FAILED_VERIFICATION)",
        "52(CURLE_GOT_NOTHING)",
        "53(CURLE_SSL_ENGINE_NOTFOUND)",
        "54(CURLE_SSL_ENGINE_SETFAILED)",
        "55(CURLE_SEND_ERROR)",
        "56(CURLE_RECV_ERROR)",
        "57(CURLE_OBSOLETE57)",
        "58(CURLE_SSL_CERTPROBLEM)",
        "59(CURLE_SSL_CIPHER)",
        "60(CURLE_SSL_CACERT)",
        "61(CURLE_BAD_CONTENT_ENCODING)",
        "62(CURLE_LDAP_INVALID_URL)",
        "63(CURLE_FILESIZE_EXCEEDED)",
        "64(CURLE_USE_SSL_FAILED)",
        "65(CURLE_SEND_FAIL_REWIND)",
        "66(CURLE_SSL_ENGINE_INITFAILED)",
        "67(CURLE_LOGIN_DENIED)",
        "68(CURLE_TFTP_NOTFOUND)",
        "69(CURLE_TFTP_PERM)",
        "70(CURLE_REMOTE_DISK_FULL)",
        "71(CURLE_TFTP_ILLEGAL)",
        "72(CURLE_TFTP_UNKNOWNID)",
        "73(CURLE_REMOTE_FILE_EXISTS)",
        "74(CURLE_TFTP_NOSUCHUSER)",
        "75(CURLE_CONV_FAILED)",
        "76(CURLE_CONV_REQD)",
        "77(CURLE_SSL_CACERT_BADFILE)",
        "78(CURLE_REMOTE_FILE_NOT_FOUND)",
        "79(CURLE_SSH)",
        "80(CURLE_SSL_SHUTDOWN_FAILED)",
        "81(CURLE_AGAIN)",
        "82(CURLE_SSL_CRL_BADFILE)",
        "83(CURLE_SSL_ISSUER_ERROR)",
        "84(CURLE_FTP_PRET_FAILED)",
        "85(CURLE_RTSP_CSEQ_ERROR)",
        "86(CURLE_RTSP_SESSION_ERROR)",
        "87(CURLE_FTP_BAD_FILE_LIST)",
        "88(CURLE_CHUNK_FAILED"
    };

    if (errcode >= max_size_curl_error || errcode < 0)
        return "Unknown Reason";
    return strCurlMessage[errcode];
}
