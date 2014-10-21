// ***********************************************************************
// Filename         : ByteArray.h
// Author           : LIZHENG
// Created          : 2014-09-19
// Description      : 二进制字节流
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-09-22
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_BYTEARRAY_H
#define ZL_BYTEARRAY_H
#include <string>
#include <vector>

enum Endian
{
    BIG_ENDIAN,
    LIT_ENDIAN
};

class ByteArray
{
public:
    ByteArray(int allocSize);
    ~ByteArray();

public:    //Data Write
    void WriteBool(bool val);
    void WriteByte(char val);
    void WriteChars(const char *val);
    void WriteChars(const char *val, size_t size, int offset = 0);
    void WriteString(const std::string& val);

    template <typename Number>
    int WriteNumber(Number val)
    {
        char bytes[sizeof(val)];
        int size = NumberToBytes(val, bytes, writeEndian_);
        WriteChars(bytes, size);
        return size;
    }

public:    //Data Read
    bool ReadBool();
    char ReadByte();
    void ReadChars(char *val, size_t size);
    std::string ReadString();
    bool ReadBytes(char *val, size_t size, int offset = 0);

    template <typename Number>
    int ReadNumber(Number *val)
    {
        const int size = sizeof(*val);
        char bytes[size];
        ReadBytes(bytes, size);
        BytesToNumber(val, bytes, readEndian_);
        return size;
    }

    template <typename Number>
    Number Read()
    {
        Number val;
        const int size = sizeof(val);
        char bytes[size];
        ReadBytes(bytes, size);
        BytesToNumber(&val, bytes, readEndian_);
        return val;
    }

public:    //Property Access
    const char* Data() const
    {
        return bytesBuf_.data();
    }

    size_t Size()
    {
        return bytesBuf_.size();
    }

    size_t ReadableBytes() const   //还可以读取多少有效字节
    { 
        return writePos_ - readPos_; 
    }

    size_t WritableBytes() const   //还可以写入有效字节
    { 
        return bytesBuf_.size() - writePos_;
    }

    void SetEndian(Endian writeEndian, Endian readEndian)
    {
        writeEndian_ = writeEndian;
        readEndian_ = readEndian;
    }

private:
    template <typename T>
    int NumberToBytes(T val, char *bytes, Endian endian)
    {
        int size = sizeof(val);
        *(T *)bytes = val;
        if(endian == GetCPUEndian())
        {
            return size;
        }
        else
        {
            ReversalArray(bytes, size);
            return size;
        }
    }
    template <typename T>
    int BytesToNumber(T val, char *bytes, Endian endian)
    {
        int size = sizeof(*val);
        if(endian == GetCPUEndian())
        {
        }
        else
        {
            ReversalArray(bytes, size);
        }
        *val = *(T)bytes;
        return size;
    }

    static Endian GetCPUEndian();
    static void ReversalArray(char *bytes, size_t size);

private:
    ByteArray(const ByteArray&);
    ByteArray& operator=(const ByteArray&);

private:
    Endian      writeEndian_;
    Endian      readEndian_;

    size_t         writePos_;
    size_t         readPos_;
    std::vector<char> bytesBuf_;
};

#endif /* ZL_BYTEARRAY_H */
