// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#include <stdio.h>
#include <tchar.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;
#include <windows.h>
#include <assert.h>


const string& stringReplace(string& str, const string& strToReplace, const string& strReplaceTo);
char *stringReplace(char *str, const char *strToReplace, const char *strReplaceTo);

// UTF8����ת����GBK����
int UTF8ToGBK(const char *lpUTF8Str, char *lpGBKStr,int nGBKStrLen);
//GBK����ת����UTF8����
int GBKToUTF8(const char *lpGBKStr, char *lpUTF8Str, int nUTF8StrLen);


enum URL_TYPE{unkowurl, eastmoney, howbuy, fund123, jjmmw, qq};
