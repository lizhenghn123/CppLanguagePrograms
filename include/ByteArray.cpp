#include "ByteArray.h"

ByteArray::ByteArray(int allocSize)
{
    writePos_ = 0;
    readPos_ = 0;
    bytesBuf_ = (Byte *)malloc(allocSize);
    Endian cpuEndian = GetCPUEndian();
    SetEndian(cpuEndian, cpuEndian);
}

ByteArray::~ByteArray()
{
    //if(bytesBuf_)
    //	free(bytesBuf_);
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

void ByteArray::WriteBytes(Byte *val, int size, int offset/* = 0*/)
{
    Byte *srcByte = val + offset;
    Byte *desByte = bytesBuf_ + writePos_;
    memcpy(desByte, srcByte, size);
    writePos_ += size;
}

bool ByteArray::ReadBool()
{
    bool val = false;
    ReadBytes((Byte *)&val, 1, 0);
    return val;
}

unsigned char ByteArray::ReadByte()
{
    unsigned char *val = NULL;
    ReadBytes(val, 1);
    return *val;
}

void ByteArray::ReadBytes(Byte *val, int size, int offset/* = 0*/)
{
    Byte *desByte = val + offset;
    Byte *srcByte = bytesBuf_ + readPos_;
    memcpy(desByte, srcByte, size);
    readPos_ += size;
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
    union
    {
        short s;
        char c[sizeof(short)];
    } un;
    un.s = 0x0102;
    if(un.c[0] == 0x2)
    {
        return LIT_ENDIAN;
    }
    else
    {
        return BIG_ENDIAN;
    }
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

