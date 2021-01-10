#include "stdafx.h"
#include "AwkBase.h"

CAwkDoc::CAwkDoc(void)
{
    m_delimer = " ";
    m_nf = m_nr = 0;
    m_lineopr = NULL;
}

CAwkDoc::CAwkDoc(IAwkOneDualItr* lineopr, const string& filepath):
m_lineopr(lineopr), m_filename(filepath)
{
    m_nf = m_nr = 0; 
    m_delimer = " ";
}

CAwkDoc::~CAwkDoc(void)
{
}


bool CAwkDoc::run()
{
    //��ȡ�ļ�
    string inparamStr = FileToString(m_filename);


    //���ļ�����
    vector<string> vecLines;
    SplitStr(inparamStr, "\n", vecLines);

    if (vecLines.size() == 0)
    {
        cerr << "ERR: �ļ�(" <<  m_filename << ")Ϊ�ջ򲻴���!" << endl;
        return false;
    }

    assert(m_lineopr != NULL);
    m_lineopr->dualBegin(*this);

    for (size_t i = 0; i < vecLines.size(); i++)
    {
        m_line = vecLines[i];
        if (m_line == "")
        {
            continue;
        }


        //ɾ������ո�(Ϊ����ָ���׼��)
        string theline = m_line;
        //constrictSpace(theline);

        m_nr++;
        m_nf = SplitStr(theline, m_delimer, m_vecParts);

        if (m_vecParts.size() < 20)
        {
            m_vecParts.resize(20);
        }

        int ret = m_lineopr->dualOneLine(*this);

        //1���������� -1���쳣����
        if ( ret == 1 )
        {
            break;
        }
        else if ( ret == -1 )
        {
            return false;
        }
    }

    m_lineopr->dualEnd(*this);

    return true;
}

void CAwkDoc::setStrParam(const string& paramname, const string& paramvalue)
{
    m_strParams[paramname] = paramvalue;
}

string& CAwkDoc::getStrParam(const string& paramname)
{
    return m_strParams[paramname];
}