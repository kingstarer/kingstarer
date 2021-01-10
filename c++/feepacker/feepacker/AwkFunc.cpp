#include "stdafx.h"
#include "AwkFunc.h"

std::string& stringReplace(std::string&   input,     //   原始串   
                           const   std::string&   toReplace,     //   替换源串   
                           const   std::string&   replaceWith)     //   替换目的串   
{   
    size_t curPos = 0;   
    size_t pos;   

    if((pos = input.find(toReplace, curPos)) != std::string::npos)   
    {   
        //只进行一次替换   
        input.replace(pos, toReplace.size(), replaceWith);   
    }   

    return   input;   
} 


std::string& stringReplaceAll(std::string&   input,     //   原始串   
                              const   std::string&   toReplace,     //   替换源串   
                              const   std::string&   replaceWith)     //   替换目的串   
{   
    size_t curPos = 0;   
    size_t pos;   

    while((pos = input.find(toReplace, curPos)) != std::string::npos)   
    {   
        //只进行一次替换   
        input.replace(pos, toReplace.size(), replaceWith);   
        curPos = curPos + replaceWith.size();
    }   

    return   input;   
} 

//删除多余空格
bool constrictSpace(string& instr)
{
    const char *str = instr.c_str();
    string outstr;
    outstr.reserve(instr.size());

    while (*str)
    {
        if (*str == ' ' || *str == '\t')
        {
            outstr += ' ';

            //走到下一个非空格处
            for (str++; (*str != '\0') && ((*str == ' ') 
                || (*str == '\t')); str++)
            {
            }
        }
        else
        {
            outstr += *str++;
        }
    }

    instr = outstr;

    return true;
}

string toupper(const string& str)
{
    string tmpstr = str;
    //transform(str.begin(), str.end(), str.begin(), toupper);
    for (string::iterator it = tmpstr.begin(); it != tmpstr.end(); it++)
    {
        *it = toupper(*it);
    }

    return tmpstr;
}

string tolower(const string& str)
{
    string tmpstr = str;
    //transform(str.begin(), str.end(), str.begin(), tolower);
    for (string::iterator it = tmpstr.begin(); it != tmpstr.end(); it++)
    {
        *it = tolower(*it);
    }

    return tmpstr;
}



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
int SplitStr(const string& source, const string& delimiter, vector<string>& ve_parts)
{
    ve_parts.clear();
    size_t beg_pos = 0;
    size_t pos = string::npos;

    //源串是空，直接返回
    if (source == "")
    {
        return 0;
    }

    //分隔符是空，直接按字符拆分
    if (delimiter == "")
    {
        for (size_t i = 0; i < source.size(); i++)
        {
            ve_parts.push_back( string(1, source[i]) );
        }

        return (int) ve_parts.size();
    }

    while (pos != source.size())
    {
        //从剩下的字符串中找到第一个分隔符
        pos = source.find(delimiter, beg_pos);
        if (pos == string::npos)
        {
            //找不到则跳出循环
            break;
        }

        //将剩下的字符串的开始到下一个分隔符之前的数据做为一列
        ve_parts.push_back( source.substr(beg_pos, pos - beg_pos) );

        //将前面部分字符串截断
        beg_pos = pos + delimiter.size();
    }

    //如果字符串还有剩余，则将剩余部分全部当成一列。
    if (beg_pos != source.size())
    {
        ve_parts.push_back( source.substr(beg_pos) );
    }
    return (int) ve_parts.size();
}


string FileToString(const string& filepath)
{
    string str;
    char buffer[LINE_SIZE];
    FILE *fp = fopen(filepath.c_str(), "r");

    if (fp != NULL)
    {
        while(fgets(buffer, LINE_SIZE, fp) != NULL)
        {
            str += buffer;
        }

        fclose(fp);
    }

    return str;
}
