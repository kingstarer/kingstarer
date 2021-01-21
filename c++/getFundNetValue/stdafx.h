// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;
#include <windows.h>
#include <assert.h>


const string& stringReplace(string& str, const string& strToReplace, const string& strReplaceTo);
char *stringReplace(char *str, const char *strToReplace, const char *strReplaceTo);

// UTF8编码转换到GBK编码
int UTF8ToGBK(const char *lpUTF8Str, char *lpGBKStr,int nGBKStrLen);
//GBK编码转换到UTF8编码
int GBKToUTF8(const char *lpGBKStr, char *lpUTF8Str, int nUTF8StrLen);


enum URL_TYPE{unkowurl, eastmoney, howbuy, fund123, jjmmw, qq};
