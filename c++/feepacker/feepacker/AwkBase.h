#pragma once
#include "AwkFunc.h"

class CAwkDoc;
class IAwkOneDualItr
{
public:
    virtual int dualBegin(CAwkDoc &Doc) = 0;
    //返回0代表正常，继续处理下一条 返回1代表退出文本处理直接到End阶段 返回-1代表处理出错，直接退出  
    virtual int dualOneLine(CAwkDoc &Doc) = 0;
    virtual int dualEnd(CAwkDoc &Doc) = 0;
};

class CAwkOneDualItrBase:public IAwkOneDualItr
{
public:
    virtual int dualBegin(CAwkDoc& Doc)
    {
        return 0;
    }

    virtual int dualEnd(CAwkDoc& Doc)
    {
        return 0;
    }
};

class CAwkDoc
{
public:
    typedef map<string, string> AwkMapType;
    typedef vector<string> AwkVecType;

public:
    CAwkDoc(void);
    CAwkDoc(IAwkOneDualItr* lineopr, const string& filepath);
    virtual ~CAwkDoc(void);
    virtual bool run();

    void setFileName(const string& filepath) 
    { 
        m_filename = filepath; 
    }

    void setDelimer(const string delimer) 
    { 
        m_delimer = delimer; 
    }

    vector<string>& Parts()
    {
        return m_vecParts;
    }

    void setStrParam(const string& paramname, const string& paramvalue);
    string& getStrParam(const string& paramname);

    AwkVecType& getVecParam(const string& paramname);
    AwkMapType& getMapParam(const string& paramname);

public:

    int m_nf;
    int m_nr;
    string m_filename;
    string m_line;
    string m_delimer;
    vector<string> m_vecParts;


    map<string, string> m_strParams;
    map< string, AwkMapType > m_mpParams;
    map< string, AwkVecType > m_vecParams;

public:

    IAwkOneDualItr *m_lineopr;
};
