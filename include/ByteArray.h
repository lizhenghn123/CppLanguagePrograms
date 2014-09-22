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
    typedef unsigned char Byte;
public:
    ByteArray(int allocSize);
    virtual ~ByteArray();

public:    //Data Write
    void WriteBool(bool val);
    void WriteByte(Byte val);
    void WriteChars(char *val);
    void WriteString(const std::string& val);
    template <typename Number>
    int WriteNumber(Number val)
    {
        Byte bytes[sizeof(val)];
        int size = NumberToBytes(val, bytes, writeEndian_);
        WriteBytes(bytes, size);
        return size;
    }

public:    //Data Read
    bool ReadBool();
    Byte ReadByte();
    void ReadChars(char *val, int size);
    std::string ReadString();
    template <typename Number>
    int ReadNumber(Number *val)
    {
        int size = sizeof(*val);
        Byte *bytes = (Byte *)malloc(size);
        ReadBytes(bytes, size);
        BytesToNumber(val, bytes, readEndian_);
        free(bytes);
        return size;
    }

    template <typename Number>
    Number Read()
    {
        Number val;
        int size = sizeof(val);
        Byte *bytes = (Byte *)malloc(size);
        ReadBytes(bytes, size);
        BytesToNumber(&val, bytes, readEndian_);
        free(bytes);
        return val;
    }
public:    //Property Access
    int Size()
    {
        return writePos_;
    }

    int Available()
    {
        return bytesBuf_.size() - writePos_;
    }

    void SetEndian(Endian writeEndian, Endian readEndian)
    {
        writeEndian_ = writeEndian;
        readEndian_ = readEndian;
    }

    template <typename T>
    int NumberToBytes(T val, Byte *bytes, Endian endian)
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
    int BytesToNumber(T val, Byte *bytes, Endian endian)
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
    static void ReversalArray(Byte *bytes, int size);

private:
    bool WriteBytes(Byte *val, int size, int offset = 0);
    bool ReadBytes(Byte *val, int size, int offset = 0);

private:
    Endian      writeEndian_;
    Endian      readEndian_;

    int         writePos_;
    int         readPos_;
    std::vector<char> bytesBuf_;
};

#endif /* ZL_BYTEARRAY_H */
