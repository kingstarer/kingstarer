#pragma once

#include <string>
#include <string.h>
using namespace std;

string tolower(const string& str);
string toupper(const string& str);
bool constrictSpace(string& instr);

std::string& stringReplaceAll(std::string&   input,     //   ԭʼ��   
                              const   std::string&   toReplace,     //   �滻Դ��   
                              const   std::string&   replaceWith);     //   �滻Ŀ�Ĵ�   

std::string& stringReplace(std::string&   input,     //   ԭʼ��   
                           const   std::string&   toReplace,     //   �滻Դ��   
                           const   std::string&   replaceWith);     //   �滻Ŀ�Ĵ�   

/**
* Description		: ���ַ������ָ�����ֳɶ��д�ŵ�vector��(�����)
* @param[out] ve_parts	: ��ֽ������ڴ˴�
* @param[in] source	: Դ�ַ���
* @param[in] delimiter: �ָ���
* @return			: �ָ�������
* @note			: ���ַ���û�зָ���ʱ��Ҳ����һ�С� 
* @note			: ���ָ����ǿմ�ʱ�����ᰴ�ַ���֡���Դ�ַ����ǿմ�ʱ������0
*					: �ַ�����β�ָ�������Բ�ֽ����Ӱ�죬���ǿ�ʼ�ķָ����ᡣ
*					: ���������ģ�����ַ�������Ӣ�Ļ�ϵ�ʱ���鲻ʹ�ô˺���
* @exception
*/
int SplitStr(const string& source, const string& delimiter, vector<string>& ve_parts);

//��ȡ�ļ� �浽�ַ���
string FileToString(const string& filepath);

inline string toString(int n)
{
    char buf[64];
    sprintf(buf, "%d", n);
    return buf;
}


/*��ָ����ʽ�����ص�ǰʱ��, ��: "%Y-%m-%d %H:%M:%S" */
inline string CurrentToStr( const char * format = "%Y%m%d%H%M%S" )
{
    struct tm *ptr;
    time_t lt;
    char str[80];

    lt = time(NULL);
    ptr = localtime(&lt);

    strftime(str,100,format,ptr);
    std::string rlt(str);

    return ( str );
}

