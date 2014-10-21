#include "ByteArray.h"
#include "ByteArray.h"

ByteArray::ByteArray(int allocSize)
{
    writePos_ = 0;
    readPos_ = 0;
    bytesBuf_.reserve(allocSize <= 0 ? 1 : allocSize);
    bytesBuf_.clear();
    Endian cpuEndian = GetCPUEndian();
    SetEndian(cpuEndian, cpuEndian);
}

ByteArray::~ByteArray()
{
    std::vector<char>().swap(bytesBuf_);
}

void ByteArray::WriteBool(bool val)
{
    WriteChars((char *)&val, 1, 0);
}

void ByteArray::WriteByte(char val)
{
    WriteChars(&val, 1);
}

void ByteArray::WriteChars(const char *val)
{
    WriteChars(val, (int)strlen(val), 0);
}

void ByteArray::WriteString(const std::string& val)
{
    WriteNumber(val.size());
    WriteChars(val.c_str(), val.size(), 0);
}

void ByteArray::WriteChars(const char *val, size_t size, int offset/* = 0*/)
{
    const char *srcByte = val + offset;
    if((int)bytesBuf_.size() < (writePos_ + size))
    {
        bytesBuf_.resize(writePos_ + size);  //每次只按需扩展所需大小，避免多分配内存, 也可直接resize 2倍大小，就像stl那样
    }
    ::memcpy(&bytesBuf_[writePos_], srcByte, size);
    writePos_ += size;
}

bool ByteArray::ReadBool()
{
    bool val = false;
    ReadBytes((char *)&val, 1, 0);
    return val;
}

char ByteArray::ReadByte()
{
    char val;
    ReadBytes(&val, 1);
    return val;
}

bool ByteArray::ReadBytes(char *val, size_t size, int offset/* = 0*/)
{
    char *dstByte = val + offset;
    if(readPos_ + size > (int)bytesBuf_.size())
    {
        *dstByte = 0;
        return false;
    }
    ::memcpy(dstByte, &bytesBuf_[readPos_], size);
    readPos_ += size;
    return true;
}

void ByteArray::ReadChars(char *val, size_t size)
{
    ReadBytes((char *)val, size, 0);
}

std::string ByteArray::ReadString()
{
    int size;
    ReadNumber(&size);
    char *chars = (char *)malloc(size + 1);
    memset(chars, 0, size + 1);
    ReadBytes((char *)chars, size, 0);
    std::string str(chars);
    free(chars);
    return str;
}

Endian ByteArray::GetCPUEndian()
{
    union w
    {
        int i;
        char c;
    } u;
    u.i = 1;

    if(u.c == 1)
        return LIT_ENDIAN;
    else
        return BIG_ENDIAN;
}

void ByteArray::ReversalArray(char *bytes, size_t size)
{
    for(size_t i = 0; i < size / 2; i++)
    {
        char tb;
        tb = *(bytes + i);
        *(bytes + i) = *(bytes + size - i - 1);
        *(bytes + size - i - 1) = tb;
    }
}

