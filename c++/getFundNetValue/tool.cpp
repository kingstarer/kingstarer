#include "stdafx.h"




bool setClipboard(const string& str)
{
	if (!OpenClipboard(NULL))   
	{
		return false;
	}

	size_t nCount = str.size();  
	HGLOBAL hGlobalMem = GlobalAlloc(GMEM_MOVEABLE, (nCount+1) * sizeof(TCHAR));

	if (!hGlobalMem)  
	{   
		CloseClipboard();   
		return false;  
	}

	EmptyClipboard();  

	LPTSTR lpszStr= (LPTSTR) GlobalLock(hGlobalMem);  
	memcpy(lpszStr, str.c_str(), nCount * sizeof(TCHAR));  
	lpszStr[nCount] = (TCHAR) 0;  
	GlobalUnlock(hGlobalMem);

#ifdef UNICODE  
	SetClipboardData(CF_UNICODETEXT, hGlobalMem);
#else  
	SetClipboardData(CF_TEXT, hGlobalMem);
#endif  

	CloseClipboard();

	return true;
}

// 获取json的值
const string getValueMh(const string& strIn)
{
	size_t nPos = strIn.find(":\"");
	if (nPos == string::npos)
	{
		return "";
	}

	string res = strIn.substr(nPos + 2);

	if (res[res.size() - 1] == '\"')
	{
		res.resize(res.size() - 1);
	}

	return res;
}



//trim
void removeCharHeadTail(string& str, const char trimChr)
{
	size_t begPos = 0;
	size_t endPos = string::npos;
	size_t nStrLen = str.size();

	if (nStrLen == 0)
	{
		return;
	}

	if (str[0] == trimChr) 
	{
		begPos = 1;
	}

	if (str[nStrLen - 1] == trimChr)
	{
		endPos = nStrLen - begPos - 1;
	}

	str = str.substr(begPos, endPos);
}



//字符串分割
size_t splitToVector(const char *src, const char *separator, vector<string>& vecOut)
{
	const char *pFound, *pBegin = src;
	const size_t nSeparatorLen = strlen(separator);

	vecOut.clear();

	while ( NULL != (pFound = strstr(pBegin, separator)) )
	{
		vecOut.push_back(string(pBegin, pFound));
		pBegin = pFound + nSeparatorLen;
	}

	if (*pBegin != '\0')
	{
		vecOut.push_back(pBegin);
	}

	return vecOut.size();
}



int getHtml(const string& url, OUT string& strHtml) 
{
	string strCmd = "curl --compressed \"";
	strCmd += url;
	strCmd += '"';

	string tmpHtml = "tmpFund.html";
	strCmd += " -o " + tmpHtml;
	system(strCmd.c_str());

	FILE *fp = fopen(tmpHtml.c_str(), "r");
	if (fp == NULL)
	{
		perror("无法打开临时文件，请确认文件夹是否只读。");
		return -1;
	}

	char buffer[4097];
	size_t nRead; 
	while ((nRead = fread(buffer, 1, sizeof(buffer) - 1, fp)) != 0)
	{
		buffer[nRead] = '\0';
		strHtml += buffer;
	}
	fclose(fp); 

	if (strHtml == "")
	{
		fprintf(stderr, "获取页面失败，请检查是否网络连接有问题。\n");
		return -2;
	}

	return 0;
}

//根据关键字截取字符串
string substrByKeyWord(const string& input, const string& begKeyWord, const string& endKeyWord) 
{
	size_t begPos = input.find(begKeyWord);
	if (begPos == string::npos)
	{
		begPos = 0;
	}
	else 
	{
		begPos += begKeyWord.length();
	}
	
	size_t nCnt = string::npos;
	size_t endPos = input.find(endKeyWord, begPos);
	if (endPos != string::npos) 
	{
		nCnt = endPos - begPos;
	}
	
	return input.substr(begPos, nCnt);

}

