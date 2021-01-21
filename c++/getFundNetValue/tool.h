#pragma once

#include <algorithm>
#include <string>
#include <map>
using namespace std;

//�޸ļ�����
bool setClipboard(const string& str);

// ��ȡjson��ֵ
const string getValueMh(const string& strIn);

//trim
void removeCharHeadTail(string& str, const char trimChr);


//�ַ����ָ�
size_t splitToVector(const char *src, const char *separator, vector<string>& vecOut);

//��ȡurl����ҳ������
int getHtml(const string& url, OUT string& strHtml) ;

//���ݹؼ��ֽ�ȡ�ַ���
string substrByKeyWord(const string& input, const string& begKeyWord, const string& endKeyWord);

