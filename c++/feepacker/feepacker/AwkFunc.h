#pragma once

#include <string>
#include <string.h>
using namespace std;

string tolower(const string& str);
string toupper(const string& str);
bool constrictSpace(string& instr);

std::string& stringReplaceAll(std::string&   input,     //   原始串   
                              const   std::string&   toReplace,     //   替换源串   
                              const   std::string&   replaceWith);     //   替换目的串   

std::string& stringReplace(std::string&   input,     //   原始串   
                           const   std::string&   toReplace,     //   替换源串   
                           const   std::string&   replaceWith);     //   替换目的串   

/**
* Description		: 将字符串按分隔符拆分成多列存放到vector中(先清空)
* @param[out] ve_parts	: 拆分结果存放在此处
* @param[in] source	: 源字符串
* @param[in] delimiter: 分隔符
* @return			: 分割结果列数
* @note			: 当字符串没有分隔符时，也会拆出一列。 
* @note			: 当分隔符是空串时，将会按字符拆分。当源字符串是空串时，返回0
*					: 字符串结尾分隔符不会对拆分结果有影响，但是开始的分隔符会。
*					: 不考虑中文，如果字符串是中英文混合的时建议不使用此函数
* @exception
*/
int SplitStr(const string& source, const string& delimiter, vector<string>& ve_parts);

//读取文件 存到字符串
string FileToString(const string& filepath);

inline string toString(int n)
{
    char buf[64];
    sprintf(buf, "%d", n);
    return buf;
}


/*以指定格式串返回当前时间, 如: "%Y-%m-%d %H:%M:%S" */
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

