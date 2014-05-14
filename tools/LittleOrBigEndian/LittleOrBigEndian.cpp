// ***********************************************************************
// Filename         : LittleOrBigEndian.cpp
// Author           : LIZHENG
// Created          : 2014-05-14
// Description      : CPU大小端判断
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-05-14
// 
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************

#include <iostream>

int IsLittleEndian( )
{
	//Little-endian模式的CPU对操作数的存放方式是从低字节到高字节，而Big-endian模式对操作数的存放方式是从高字节到低字节。
	//[大小端存储问题]: 小端方式中（i占至少两个字节的长度）,i所分配的内存最小地址那个字节中就存着1，其他字节是0.
	//大端的话则1在i的最高地址字节处存放，char是一个字节，所以强制将char型量p指向i则p指向的一定是i的最低地址，
	//那么就可以判断p中的值是不是1来确定是不是小端。
	//若处理器是Big_endian的，则返回0；若是Little_endian的，则返回1

	//联合体union的存放顺序是所有成员都从低地址开始存放
	union w {
		int i;
		char c;
	} u;
	u.i = 1;
	return(u.c ==1);
}

int  IsLittleEndianEx() 
{ 
	int i = 0x1; 
	return *(char *)&i == 0x1; 
}

int main()
{
	printf("is little endian: %d\n", IsLittleEndian());
	printf("is little endian: %d\n", IsLittleEndianEx());
	system("pause");
	return 0;
}