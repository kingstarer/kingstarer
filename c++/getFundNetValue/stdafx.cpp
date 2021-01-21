// stdafx.cpp : 只包括标准包含文件的源文件
// getFundNetValue.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

const string& stringReplace(string& str, const string& strToReplace, const string& strReplaceTo)
{
	size_t pos = str.find(strToReplace);
	if (pos != string::npos)
	{
		str.replace(pos, strToReplace.size(), strReplaceTo);
	}

	return str;
}

char *stringReplace(char *str, const char *strToReplace, const char *strReplaceTo)
{
	size_t cntOld = strlen(strToReplace);
	size_t cntNew = strlen(strReplaceTo);

	if (cntNew == cntOld && strcmp(strReplaceTo, strToReplace) == 0)
	{
		return str;
	}

	char *p = strstr(str, strToReplace);
	if (p == NULL)
	{
		return str;
	}

	if (cntOld < cntNew)
	{
		//先找到原字符串尾部\0的位置
		char *endpos = p;
		while(*endpos)
		{
			endpos++;
		}

		//将字符串从尾部起，每个字符向后移动cntNew - cntOld个位置
		while (endpos >= p + cntOld)
		{
			*(endpos + cntNew - cntOld) = *endpos;
			endpos--;
		}

		memcpy(p, strReplaceTo, cntNew);

	}
	else if (cntNew == cntOld)
	{
		memcpy(p, strReplaceTo, cntNew);
	}
	else
	{
		memcpy(p, strReplaceTo, cntNew);
		memmove(p + cntNew, p + cntOld, strlen(p + cntOld) + 1);
	}

	return str;
}

//GBK编码转换到UTF8编码
int GBKToUTF8(const char *lpGBKStr, char *lpUTF8Str, int nUTF8StrLen)
{
	wchar_t *lpUnicodeStr = NULL;
	int nRetLen = 0;

	if(!lpGBKStr)  //如果GBK字符串为NULL则出错退出
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, lpGBKStr, -1, NULL, NULL);  //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, lpGBKStr, -1, lpUnicodeStr, nRetLen);  //转换到Unicode编码
	if(!nRetLen)  //转换失败则出错退出
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL);  //获取转换到UTF8编码后所需要的字符空间长度

	if(!lpUTF8Str)  //输出缓冲区为空则返回转换后需要的空间大小
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return nRetLen;
	}

	if(nUTF8StrLen < nRetLen)  //如果输出缓冲区长度不够则退出
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,(char *)lpUTF8Str,nUTF8StrLen,NULL,NULL);  //转换到UTF8编码

	if(lpUnicodeStr)
		delete []lpUnicodeStr;

	return nRetLen;
}


// UTF8编码转换到GBK编码
int UTF8ToGBK(const char *lpUTF8Str, char *lpGBKStr,int nGBKStrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if(!lpUTF8Str)  //如果UTF8字符串为NULL则出错退出
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_UTF8, 0, lpUTF8Str, -1, NULL, NULL);  //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,lpUnicodeStr,nRetLen);  //转换到Unicode编码
	if(!nRetLen)  //转换失败则出错退出
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,NULL,NULL,NULL,NULL);  //获取转换到GBK编码后所需要的字符空间长度

	if(!lpGBKStr)  //输出缓冲区为空则返回转换后需要的空间大小
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return nRetLen;
	}

	if(nGBKStrLen < nRetLen)  //如果输出缓冲区长度不够则退出
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,(char *)lpGBKStr,nRetLen,NULL,NULL);  //转换到GBK编码

	if(lpUnicodeStr)
		delete []lpUnicodeStr;

	return nRetLen;
}



char *stringReplaceAll(char *str, const char *strToReplace, const char *strReplaceTo)
{
	size_t cntOld = strlen(strToReplace);
	size_t cntNew = strlen(strReplaceTo);

	if (cntNew == cntOld && strcmp(strReplaceTo, strToReplace) == 0)
	{
		return str;
	}

	if (*strToReplace == '\0')
	{
		return str;
	}

	char *p = strstr(str, strToReplace);
	if (p == NULL)
	{
		return str;
	}

	vector<char *> vecStrPlace;
	while (p != NULL)
	{
		vecStrPlace.push_back(p);
		p = strstr(p + cntOld + 1, strToReplace);
	}

	if (cntOld < cntNew)
	{
		//取得尚未移动的最后一个字符位置（包括\0）
		char *pEnd = p + strlen(p) + 1;
		size_t n = vecStrPlace.size();
		int nMove = n * (cntNew - cntOld);
		while (nMove > 0)
		{
			//取得尚未移动的最后一个字符串起始位置
			char *p = vecStrPlace[n - 1] + cntOld;
			//将该位置起至最后一个需要移动的字符一起往后移
			memmove(p, p + nMove, pEnd - p + 1);
			memcpy(p + nMove - cntNew, strReplaceTo, cntNew);
			pEnd = vecStrPlace[n - 1] - 1;
			nMove -= (cntNew - cntOld);
			n--;
		}	
	}
	else if (cntNew == cntOld)
	{
		for (size_t i = 0; i < vecStrPlace.size(); i++)
		{
			memcpy(vecStrPlace[i], strReplaceTo, cntNew);
		}
	}
	else
	{
		p = vecStrPlace[0];
		for (size_t i = 0; i < vecStrPlace.size() - 1; i++)
		{
			memcpy(p, strReplaceTo, cntNew);
			p += cntNew;
			memmove(p, vecStrPlace[i] + cntOld, vecStrPlace[i + 1] - vecStrPlace[i] - cntOld);
			p += vecStrPlace[i + 1] - vecStrPlace[i] - cntOld;
		}
		memcpy(p, strReplaceTo, cntNew);
		p += cntNew;
		memmove(p, vecStrPlace[vecStrPlace.size() - 1] + cntOld, strlen(vecStrPlace[vecStrPlace.size() - 1] + cntOld) + 1);
	}

	return str;
}
