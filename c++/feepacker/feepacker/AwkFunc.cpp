#include "stdafx.h"
#include "AwkFunc.h"

std::string& stringReplace(std::string&   input,     //   ԭʼ��   
                           const   std::string&   toReplace,     //   �滻Դ��   
                           const   std::string&   replaceWith)     //   �滻Ŀ�Ĵ�   
{   
    size_t curPos = 0;   
    size_t pos;   

    if((pos = input.find(toReplace, curPos)) != std::string::npos)   
    {   
        //ֻ����һ���滻   
        input.replace(pos, toReplace.size(), replaceWith);   
    }   

    return   input;   
} 


std::string& stringReplaceAll(std::string&   input,     //   ԭʼ��   
                              const   std::string&   toReplace,     //   �滻Դ��   
                              const   std::string&   replaceWith)     //   �滻Ŀ�Ĵ�   
{   
    size_t curPos = 0;   
    size_t pos;   

    while((pos = input.find(toReplace, curPos)) != std::string::npos)   
    {   
        //ֻ����һ���滻   
        input.replace(pos, toReplace.size(), replaceWith);   
        curPos = curPos + replaceWith.size();
    }   

    return   input;   
} 

//ɾ������ո�
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

            //�ߵ���һ���ǿո�
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
int SplitStr(const string& source, const string& delimiter, vector<string>& ve_parts)
{
    ve_parts.clear();
    size_t beg_pos = 0;
    size_t pos = string::npos;

    //Դ���ǿգ�ֱ�ӷ���
    if (source == "")
    {
        return 0;
    }

    //�ָ����ǿգ�ֱ�Ӱ��ַ����
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
        //��ʣ�µ��ַ������ҵ���һ���ָ���
        pos = source.find(delimiter, beg_pos);
        if (pos == string::npos)
        {
            //�Ҳ���������ѭ��
            break;
        }

        //��ʣ�µ��ַ����Ŀ�ʼ����һ���ָ���֮ǰ��������Ϊһ��
        ve_parts.push_back( source.substr(beg_pos, pos - beg_pos) );

        //��ǰ�沿���ַ����ض�
        beg_pos = pos + delimiter.size();
    }

    //����ַ�������ʣ�࣬��ʣ�ಿ��ȫ������һ�С�
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
