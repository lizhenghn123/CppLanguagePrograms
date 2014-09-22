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
    WriteBytes((Byte *)&val, 1, 0);
}

void ByteArray::WriteByte(Byte val)
{
    WriteBytes(&val, 1);
}

void ByteArray::WriteChars(char *val)
{
    WriteBytes((Byte *)val, (int)strlen(val), 0);
}

void ByteArray::WriteString(const std::string& val)
{
    WriteNumber(val.size());
    WriteBytes((Byte *)val.c_str(), val.size(), 0);
}

bool ByteArray::WriteBytes(Byte *val, int size, int offset/* = 0*/)
{
    Byte *srcByte = val + offset;
    if((int)bytesBuf_.size() < (writePos_ + size))
    {
        bytesBuf_.resize(writePos_ + size);  //每次只按需扩展所需大小，避免多分配内存, 也可直接resize 2倍大小，就像stl那样
    }
    ::memcpy(&bytesBuf_[writePos_], srcByte, size);
    writePos_ += size;
    return true;
}

bool ByteArray::ReadBool()
{
    bool val = false;
    ReadBytes((Byte *)&val, 1, 0);
    return val;
}

unsigned char ByteArray::ReadByte()
{
    Byte val;
    ReadBytes(&val, 1);
    return val;
}

bool ByteArray::ReadBytes(Byte *val, int size, int offset/* = 0*/)
{
    Byte *dstByte = val + offset;
    if(readPos_ + size > (int)bytesBuf_.size())
    {
        *dstByte = 0;
        return false;
    }
    ::memcpy(dstByte, &bytesBuf_[readPos_], size);
    readPos_ += size;
    return true;
}

void ByteArray::ReadChars(char *val, int size)
{
    ReadBytes((Byte *)val, size, 0);
}

std::string ByteArray::ReadString()
{
    int size;
    ReadNumber(&size);
    char *chars = (char *)malloc(size + 1);
    memset(chars, 0, size + 1);
    ReadBytes((Byte *)chars, size, 0);
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

void ByteArray::ReversalArray(Byte *bytes, int size)
{
    for(int i = 0; i < size / 2; i++)
    {
        Byte tb;
        tb = *(bytes + i);
        *(bytes + i) = *(bytes + size - i - 1);
        *(bytes + size - i - 1) = tb;
    }
}

