// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// getFundNetValue.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

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
		//���ҵ�ԭ�ַ���β��\0��λ��
		char *endpos = p;
		while(*endpos)
		{
			endpos++;
		}

		//���ַ�����β����ÿ���ַ�����ƶ�cntNew - cntOld��λ��
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

//GBK����ת����UTF8����
int GBKToUTF8(const char *lpGBKStr, char *lpUTF8Str, int nUTF8StrLen)
{
	wchar_t *lpUnicodeStr = NULL;
	int nRetLen = 0;

	if(!lpGBKStr)  //���GBK�ַ���ΪNULL������˳�
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, lpGBKStr, -1, NULL, NULL);  //��ȡת����Unicode���������Ҫ���ַ��ռ䳤��
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //ΪUnicode�ַ����ռ�
	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, lpGBKStr, -1, lpUnicodeStr, nRetLen);  //ת����Unicode����
	if(!nRetLen)  //ת��ʧ��������˳�
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL);  //��ȡת����UTF8���������Ҫ���ַ��ռ䳤��

	if(!lpUTF8Str)  //���������Ϊ���򷵻�ת������Ҫ�Ŀռ��С
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return nRetLen;
	}

	if(nUTF8StrLen < nRetLen)  //���������������Ȳ������˳�
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,(char *)lpUTF8Str,nUTF8StrLen,NULL,NULL);  //ת����UTF8����

	if(lpUnicodeStr)
		delete []lpUnicodeStr;

	return nRetLen;
}


// UTF8����ת����GBK����
int UTF8ToGBK(const char *lpUTF8Str, char *lpGBKStr,int nGBKStrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if(!lpUTF8Str)  //���UTF8�ַ���ΪNULL������˳�
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_UTF8, 0, lpUTF8Str, -1, NULL, NULL);  //��ȡת����Unicode���������Ҫ���ַ��ռ䳤��
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //ΪUnicode�ַ����ռ�
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,lpUnicodeStr,nRetLen);  //ת����Unicode����
	if(!nRetLen)  //ת��ʧ��������˳�
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,NULL,NULL,NULL,NULL);  //��ȡת����GBK���������Ҫ���ַ��ռ䳤��

	if(!lpGBKStr)  //���������Ϊ���򷵻�ת������Ҫ�Ŀռ��С
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return nRetLen;
	}

	if(nGBKStrLen < nRetLen)  //���������������Ȳ������˳�
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,(char *)lpGBKStr,nRetLen,NULL,NULL);  //ת����GBK����

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
		//ȡ����δ�ƶ������һ���ַ�λ�ã�����\0��
		char *pEnd = p + strlen(p) + 1;
		size_t n = vecStrPlace.size();
		int nMove = n * (cntNew - cntOld);
		while (nMove > 0)
		{
			//ȡ����δ�ƶ������һ���ַ�����ʼλ��
			char *p = vecStrPlace[n - 1] + cntOld;
			//����λ���������һ����Ҫ�ƶ����ַ�һ��������
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
