#include <iostream>
#include "HttpRestClient.h"
#include <assert.h>
#include "base/GBKtoUTF8.h"
using namespace std;

int main()
{
    {
        HttpRestClient req(true, true);
        std::string s = "阳光汽车";
        s =  zl::base::convertGBKToUtf8(s);
        string url = "http://192.168.9.2:8080/szly/parser?line=" + s;
        url += "&userid=5345sd233345xdgdfgdfg345sdgdfg23";
        req.setUserAgent("MyTestClient");
        req.get(url.c_str());  // 自动化所tts
        //req.get("http://192.168.14.104:80/tts.php?data=番禺区&speed=5&decode=6", "1.pcm");  // 捷通tts

        cout << req.getHeader() << "\n";
        cout << req.getBody() << "\n";
        system("pause");
    }
    std::cout << zl::base::convertGBKToUtf8("姚剑冲的号码是28972828") << "\n";
    {
        HttpRestClient req(true, true);
        req.setUserAgent("MyTestClient");
        req.get("http://192.168.14.215:80/tts_new.php?synth=姚剑冲的号码是28972828", "1.pcm");  // 自动化所tts
        //req.get("http://192.168.14.104:80/tts.php?data=番禺区&speed=5&decode=6", "1.pcm");  // 捷通tts
        
        cout << req.getHeader() << "\n";
        cout << req.getBody() << "\n";
        system("pause");
    }
    {
        //std::string s = "北京您好";
        std::string s = "《财政部 国家税务总局关于继续提高成品油消费税的通知》（财税〔2015〕11号）规定：将汽油、石脑油、溶剂油和润滑油的消费税单位税额由1.4元/升提高到1.52元/升；将柴油、航空煤油、燃料油的消费税单位税额由1.1元/升提高到1.2元/升。";

        cout << s << "\n";
        cout << zl::base::convertUtf8ToGBK(s) << "\n";
        cout << zl::base::convertGBKToUtf8(s) << "\n";

        s = zl::base::convertUtf8ToGBK(s);
        std::string url("http://219.234.149.147/tts.php?data=《财政部国家税务总局关于继续提高成品油消费税的通知》（财税〔2015〕11号）规定：将润滑油的消费税单位税额由1.4元/升提高到 1.52元/升；&speed=3&decode=1");
        //url += s;

        //url = zl::base::ConvertUtf8ToGBK(url);
        //url += "&speed=5&decode=6";
        cout << url << "\n";
        HttpRestClient req(false, true);
        req.get(url.c_str(), "ttt.pcm");
        cout << req.getHeader() << "\n\n";
        return 0;
        //cout << req.getBody() << "\n\n";
        {
            string m_strResponse(req.getBody());
            cout << m_strResponse.size() << "\n";

            short *src=(short *)m_strResponse.c_str();
            int len = m_strResponse.size()/2;
            short* Buf_8k = (short *)malloc(len*sizeof(short));
            printf("ParseTtsVoice 2.2[%d]\n", len);
            for(int i = 0;i<len; i++)
            {
                if(i > 16630)
                    printf("ParseTtsVoice 2.3[%d]\n", i);
                assert(src);
                assert((Buf_8k+i));
                memcpy(Buf_8k+i,src,sizeof(short));
                src+=2;
            }
            FILE* fp = fopen("1.pcm", "wb");
            printf("ParseTtsVoice 3\n");
            fwrite(Buf_8k, 2, len/2, fp);
            if(Buf_8k)
            {
                free(Buf_8k);
                Buf_8k = NULL;
            }
            fclose(fp);
        }
        system("pause");
    }
    {
        HttpRestClient req(false, false);
        req.get("http://www.baidu.com");
        assert(req.getHeader().empty());
        assert(req.getBody().empty());
        system("pause");
    }
    {
        HttpRestClient req(true, false);
        req.get("http://www.baidu.com");
        cout << req.getHeader() << "\n";
        assert(req.getBody().empty());
        system("pause");
    }
    {
        HttpRestClient req(false, true);
        req.get("http://www.baidu.com");
        assert(req.getHeader().empty());
        cout << req.getBody() << "\n";
        system("pause");
    }
    {
        HttpRestClient req(true, true);
        req.addHeader("User-Agent", "Mozilla/4.04[en](Win95;I;Nav)");
        req.get("https://www.baidu.com");
        cout << req.getHeader() << "\n";
        cout << req.getBody() << "\n";
        system("pause");
    }

    cout << "###### GAME OVER ######\n";
    system("pause");
}