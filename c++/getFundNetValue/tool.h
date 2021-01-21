#pragma once

#include <algorithm>
#include <string>
#include <map>
using namespace std;

//修改剪贴板
bool setClipboard(const string& str);

// 获取json的值
const string getValueMh(const string& strIn);

//trim
void removeCharHeadTail(string& str, const char trimChr);


//字符串分割
size_t splitToVector(const char *src, const char *separator, vector<string>& vecOut);

//获取url返回页面数据
int getHtml(const string& url, OUT string& strHtml) ;

//根据关键字截取字符串
string substrByKeyWord(const string& input, const string& begKeyWord, const string& endKeyWord);

