#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "../../include/osDefine.h"
#ifdef OS_WINDOWS
#include <stdlib.h>
#elif defined(OS_LINUX)
#include <errno.h>
#endif

//原  型: char *strcpy(char* dest, const char *src);
//功  能: 把从src地址开始且含有NULL结束符的字符串复制到以dest开始的地址空间
//说  明:
void test_strcpy()
{
    char string[10];
    char *str1 = "abcdefghi";
    strcpy(string, str1);
    printf("%s\n", string);
}

//原  型: char *stpcpy(char *destin, char *source);
//功  能: 把src所指由NULL结束的字符串复制到dest所指的数组中
//说  明:
//兼容性：linux下特有
void test_stpcpy()
{
    char *str1 = "abcdefghi";
#ifdef OS_WINDOWS
    printf("There is no ==strcpy== in windows platform\n");
#elif defined(OS_LINUX)
    char string[10];
    stpcpy(string, str1);
    printf("%s\n", string);
#endif
}

//原  型: char* strncpy(char* dest,char* src,size_t num);
//功  能: 复制src中的内容到dest，复制多少由num的值决定，返回指向dest的指针
//说  明: 如果遇到null字符（'\0'），且还没有到num个字符时，就用（num - n）（n是遇到null字符前已经有的非null字符个数）个null字符附加到destination
//        注意：并不是添加到destination的最后，而是紧跟着由source中复制而来的字符后面
void test_strncpy()
{
    char string[10];
    char *str1 = "abcdefghi";
    strncpy(string, str1, 3);
    string[3] = '\0';
    printf("%s\n", string);
    char chardes[] = "Hello,iam!";
    char charsource[] = "abc\0def";
    strncpy(chardes, charsource, 5);
    printf("%s\n", chardes);
}

//原  型: char *strcat(char *dest,char *src);
//功  能: 把src所指字符串添加到dest结尾处(覆盖dest结尾处的'\0')并添加'\0'
//说  明: src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串
void test_strcat()
{
    char destination[25];
    char *blank = " ", *c = "C++", *Borland = "Borland";
    strcpy(destination, Borland);
    strcat(destination, blank);
    strcat(destination, c);
    printf("%s\n", destination);
}

//原  型: int strcmp(const char *s1,const char * s2);
//功  能: 比较字符串s1和s2
//说  明: 按ASCII值大小相比较
void test_strcmp()
{
    char *buf1 = "aaa", *buf2 = "bbb", *buf3 = "ccc";
    int ptr;
    ptr = strcmp(buf2, buf1);
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 1\n");
    else
        printf("buffer 2 is less than buffer 1\n");
    ptr = strcmp(buf2, buf3);
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 3\n");
    else
        printf("buffer 2 is less than buffer 3\n");
}

//原  型: int stricmp(char *str1, char *str2);
//功  能: 比较两个串,大小写不敏感
//说  明:
//兼容性：Windows特有，在有些linux系统下没有该函数的定义
void test_stricmp()
{
#ifdef OS_WINDOWS
    char *buf1 = "BBB", *buf2 = "bbb";
    int ptr = stricmp(buf2, buf1);
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 1\n");
    else if (ptr < 0)
        printf("buffer 2 is less than buffer 1\n");
    else if (ptr == 0)
        printf("buffer 2 equals buffer 1\n");
#elif defined(OS_LINUX)
    printf("There is no ==stricmp== in windows platform\n");
#endif
}

//原  型: int strnicmp(char *str1, char *str2, unsigned maxlen);
//功  能: 将一个字符串中的一部分与另一个字符串比较, 大小写不敏感
//说  明:
//兼容性：Windows特有，在有些linux系统下没有该函数的定义
void test_strncmpi()
{
#ifdef OS_WINDOWS
    char *buf1 = "BBB", *buf2 = "bbb";
    int ptr = strnicmp(buf2, buf1, strlen(buf1));
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 1\n");
    else if (ptr < 0)
        printf("buffer 2 is less than buffer 1\n");
    else if (ptr == 0)
        printf("buffer 2 equals buffer 1\n");
#elif defined(OS_LINUX)
    printf("There is no ==stricmp== in windows platform\n");
#endif
}

//原  型: int stricmp(char *s1,char * s2);
//功  能: 比较两个串,大小写不敏感
//说  明:
//兼容性：Windows特有，在有些linux系统下没有该函数的定义
void test_strcmpi()
{
#ifdef OS_WINDOWS
    char *buf1 = "BBB", *buf2 = "bbb";
    int ptr = strcmpi(buf2, buf1);
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 1\n");
    else if (ptr < 0)
        printf("buffer 2 is less than buffer 1\n");
    else if (ptr == 0)
        printf("buffer 2 equals buffer 1\n");
#elif defined(OS_LINUX)
    printf("There is no ==strcmpi== in windows platform\n");
#endif
}

//原  型: int strncmp(char *str1, char *str2, int maxlen);
//功  能: 比较s1和s2字符串的前maxlen个字符，大小写敏感
//说  明:
//兼容性：Windows特有，在有些linux系统下没有该函数的定义
void test_strncmp()
{
    char *buf1 = "aaabbb", *buf2 = "bbbccc", *buf3 = "ccc";
    int ptr;
#ifdef OS_WINDOWS
    ptr = strncmp(buf2, buf1, 3);
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 1\n");
    else
        printf("buffer 2 is less than buffer 1\n");
    ptr = strncmp(buf2, buf3, 3);
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 3\n");
    else
        printf("buffer 2 is less than buffer 3\n");
    printf("There is no ==strcpy== in windows platform\n");
#elif defined(OS_LINUX)
    printf("There is no ==strncmp== in linux platform\n");
#endif
}

//原  型:  int strnicmp(char *str1, char *str2, unsigned maxlen);
//功  能: 比较两个串， 大小写不敏感
//说  明:
//兼容性：Windows特有，在有些linux系统下没有该函数的定义
void test_strnicmp()
{
#ifdef OS_WINDOWS
    char *buf1 = "BBBccc", *buf2 = "bbbccc";
    int ptr = strnicmp(buf2, buf1, 3);
    if (ptr > 0)
        printf("buffer 2 is greater than buffer 1\n");
    else if (ptr < 0)
        printf("buffer 2 is less than buffer 1\n");
    else if (ptr == 0)
        printf("buffer 2 equals buffer 1\n");
#elif defined(OS_LINUX)
    printf("There is no ==strnicmp== in windows platform\n");
#endif
}

//原  型: int strcasecmp(const char *s1, const char *s2);
//功  能: 比较字符串，大小写不敏感
//说  明:
//兼容性：Linux平台特有，相当于windows平台的stricmp
void test_strcasecmp()
{
#ifdef OS_WINDOWS
    printf("There is no ==strcasecmp== in windows platform\n");
#elif defined(OS_LINUX)
    char *a = "aBcDeF";
    char *b = "AbCdEf";
    if(!strcasecmp(a, b))
        printf("%s=%s\n", a, b);
#endif
}

//原  型: int strncasecmp(const char *s1, const char *s2, size_t n)
//功  能: 用来比较参数s1和s2字符串前n个字符，大小写不敏感
//说  明:
//兼容性：Linux平台特有，
void test_strncasecmp()
{
#ifdef OS_WINDOWS
    printf("There is no ==strncasecmp== in windows platform\n");
#elif defined(OS_LINUX)
    char *a = "aBcddfefekr";
    char *b = "AbCddfefekr";
    printf("%d\n", strncasecmp(a, b, strlen(a)));
#endif
}

//原  型: char *strchr(const char *s,char c);
//功  能: 查找字符串s中首次出现字符c的位置
//说  明: 成功则返回要查找字符第一次出现的位置，失败返回NULL
void test_strchr()
{
    char string[15];
    char *ptr, c = 'r';
    strcpy(string, "This is a string");
    ptr = strchr(string, c);
    if (ptr)
        printf("The character %c is at position: %d\n", c, ptr - string);
    else
        printf("The character was not found\n");
}

//原  型: char *strstr(const char *str1, const char *str2);
//功  能: 在串中查找指定字符串的第一次出现
//说  明: 该函数返回str2第一次在str1中的位置，如果没有找到，返回NULL
void test_strstr()
{
    char *str1 = "Borland International", *str2 = "nation", *ptr;
    ptr = strstr(str1, str2);
    printf("The substring is: %s\n", ptr);
}

//原  型: size_t strspn (const char *s,const char * accept);
//功  能: 返回字符串中第一个不在指定字符串中出现的字符下标
//说  明: 返回字符串s开头连续包含字符串accept内的字符数目
void test_strspn()
{
    char *string1 = "1234567890";
    char *string2 = "123DC8";
    int length = strspn(string1, string2);
    printf("Character where strings differ is at position %d\n", length);
}

//原  型: size_t strcspn(const char *s1,const char *s2);
//功  能: 顺序在字符串s1中搜寻与s2中字符的第一个相同字符，包括结束符NULL
//说  明: 返回这个字符在S1中第一次出现的位置
void test_strcspn()
{
    char *string1 = "1234567890";
    char *string2 = "747DC8";
    int length = strcspn(string1, string2);
    printf("Character where strings intersect is at position %d\n", length);
}

//原  型: char *strpbrk(const char *s1, const char *s2);
//功  能: 依次检验字符串s1中的字符，当被检验字符在字符串s2中也包含时，则停止检验，并返回该字符位置
//说  明: 空字符NULL不包括在内在串中查找给定字符集中的字符
void test_strpbrk()
{
    char *string1 = "abcdefghijklmnopqrstuvwxyz";
    char *string2 = "onm";
    char *ptr = strpbrk(string1, string2);
    if (ptr)
        printf("strpbrk found first character: %c\n", *ptr);
    else
        printf("strpbrk didn't find character in set\n");
}

//原  型: char *strrchr(const char *str, char c);
//功  能: 查找一个字符c在另一个字符串str中末次出现的位置
//说  明: 返回从字符串中的这个位置起，一直到字符串结束的所有字符
void test_strrchr()
{
    char string[15];
    char *ptr, c = 'r';
    strcpy(string, "This is a string");
    ptr = strrchr(string, c);
    if (ptr)
        printf("The character %c is at position: %d\n", c, ptr - string);
    else
        printf("The character was not found\n");
}

//原  型: char *strdup(char *s);
//功  能: 将串拷贝到新建的位置处
//说  明: strdup()在内部调用了malloc()为变量分配内存，不需要使用返回的字符串时，需要用free()释放相应的内存空间
void test_strdup()
{
    char *dup_str, *string = "abcde";
    dup_str = strdup(string);
    printf("%s\n", dup_str);
    free(dup_str);
}

//原  型: char *strrev(char *s);
//功  能: 把字符串s的所有字符的顺序颠倒过来（不包括空字符NULL）
//说  明: 返回指向颠倒顺序后的字符串指针
void test_strrev()
{
#ifdef OS_WINDOWS
    char *forward = "string";
    printf("Before strrev(): %s\n", forward);
    strrev(forward);
    printf("After strrev():  %s\n", forward);
#elif defined(OS_LINUX)
    printf("There is no ==strcpy== in linux platform\n");
#endif
}

//原  型: char *strset(char *str, char c);
//功  能: 将一个串中的所有字符都设为指定字符c
//说  明:
void test_strset()
{
#ifdef OS_WINDOWS
    char string[10] = "123456789";
    char symbol = 'c';
    printf("Before strset(): %s\n", string);
    strset(string, symbol);
    printf("After strset():  %s\n", string);
#elif defined(OS_LINUX)
    printf("There is no ==strset== in linux platform\n");
#endif
}

//原  型: char *strnset(char *str, char ch, unsigned n);
//功  能: 将一个串中的前n个字符都设为指定字符ch
//说  明:
void test_strnset()
{
#ifdef OS_WINDOWS
    char *string = "abcdefghijklmnopqrstuvwxyz";
    char letter = 'x';
    printf("string before strnset: %s\n", string);
    strnset(string, letter, 13);
    printf("string after  strnset: %s\n", string);
#elif defined(OS_LINUX)
    printf("There is no ==strnset== in linux platform\n");
#endif
}

//原  型: double strtod(const char *nptr,char **endptr);
//功  能: 将字符串转换为double型值
//说  明: 扫描字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，到出现非数字或字符串结束时('\0')才结束转换
void test_strtod()
{
    char input[80], *endptr;
    printf("Enter a floating point number:");
    gets(input);
    double value = strtod(input, &endptr);
    printf("The string is %s the number is %lf\n", input, value);
}

//原  型: long int strtol(const char *nptr,char **endptr,int base);
//功  能: 将参数nptr字符串根据参数base来转换成长整型数
//说  明:
void test_strtol()
{
    char *string = "87654321", *endptr;
    long lnumber;
    /* strtol converts string to long integer  */
    lnumber = strtol(string, &endptr, 10);
    printf("string = %s  long = %ld\n", string, lnumber);
}

//原  型: unsigned long strtoul(const char *nptr,char **endptr,int base);
//功  能: 将字符串根据参数base转换成无符号长整型数
//说  明: 参数base代表采用的进制方式，如base值为16则采用16进制数等当base值为0时会根据情况选择用哪种进制:
//        如果第一个字符是'0'，就判断第二字符如果是‘x’则用16进制，否则用8进制；第一个字符不是‘0’，则用10进制
void test_strtoul()
{
    char pNum[] = "0xFF";
    int a = strtoul(pNum, 0, 16);
    printf("%d\n", a);
}

//原  型: char *strtok(char s[], const char *delim);
//功  能: 分解字符串为一组字符串s为要分解的字符串，delim为分隔符字符串
//说  明:
void test_strtok()
{
    char input[16] = "abc,d";
    char *p;
    /* strtok places a NULL terminator
    in front of the token, if found */
    p = strtok(input, ",");
    if (p)   printf("%s\n", p);
    /* A second call to strtok using a NULL
    as the first parameter returns a pointer
    to the character following the token  */
    p = strtok(NULL, ",");
    if (p)   printf("%s\n", p);
}

//原  型: char *strupr(char *str);
//功  能: 将串中的小写字母转换为大写字母
//说  明:
//兼容性：Windows平台VC下函数
void test_strupr()
{
#ifdef OS_WINDOWS
    char *string = "abcdefghijklmnopqrstuvwxyz", *ptr;
    /* converts string to upper case characters */
    ptr = strupr(string);
    printf("%s\n", ptr);
#elif defined(OS_LINUX)
    printf("There is no ==strupr== in windows platform\n");
#endif
}

//原  型: void swab (char *from, char *to, int nbytes);
//功  能: 交换相临的两个字节
//说  明:
void test_swab()
{
    char source[15] = "lizheng";//"rFna koBlrna d";
    char target[15] = {'\0'};
    swab(source, target, strlen(source));
    printf("This is target: %s\n", target);
}

//原  型: char *strerror(int errnum);
//功  能: 返回指向错误信息字符串的指针
//说  明:
void test_strerror()
{
    char *buffer = strerror(errno);
    printf("Error: %s\n", buffer);
}

int main(int argc, char *argv[])
{
    test_swab();
    system("pause");
    return 0;
}