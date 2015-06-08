// ***********************************************************************
// Filename         : Bitset.h
// Author           : LIZHENG
// Created          : 2015-06-07
// Description      : 
//
// Last Modified By : LIZHENG
// Last Modified On : 2015-06-07
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_BITSET_H
#define ZL_BITSET_H
#include <stdio.h>

namespace zl
{
    template<typename T>
    class Bitset
    {
    public:
        Bitset(T v) : value_(v)   // may be use ref ?
        {
        }

        void set(unsigned int pos)
        {
            ((char *)(&value_))[(pos) / 8] |= (0x01 << ((pos) % 8));
        }

        void clr(unsigned int pos)
        {
            ((char *)(&value_))[(pos) / 8] &= ~(0x01 << ((pos) % 8));
        }

        int test(unsigned int pos)
        {
            return (((char *)(&value_))[(pos) / 8] >> ((pos) % 8)) & 0x01;
        }

        operator T()
        {
            return value_;
        }

        void dump()
        {
            const static size_t BITS_PER_BYTE = 8;
            for (size_t i = 0; i < BITS_PER_BYTE * sizeof(value_); ++i) {
                printf("%d", test(i));
                if ((i + 1) % BITS_PER_BYTE == 0)
                    printf(" ");
            }
            printf("\n");
        }

    private:
        T value_;
    };

}
#endif  /* ZL_BITSET_H */