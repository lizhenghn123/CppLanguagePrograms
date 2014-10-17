// ***********************************************************************
// Filename         : FileUtil.h
// Author           : LIZHENG
// Created          : 2014-07-01
// Description      : 文件、目录相关工具函数
//
// Last Modified By : LIZHENG
// Last Modified On : 2014-10-17
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_FILEUTIL_H
#define ZL_FILEUTIL_H
#include "Define.h"
#include <sstream>
NAMESPACE_ZL_START

// ------------------------  file utils  ------------------------
bool  IsDirectory(const char *dir);

bool  CreateRecursionDir(const char *dir);

bool  IsFileExist(const char *filepath);
long  GetFileSize(FILE *file);
long  GetFileSize(const char *filepath);
bool  GetFileData(const char *filepath, std::string& buf);


NAMESPACE_ZL_END
#endif /* ZL_FILEUTIL_H */
