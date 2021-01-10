#include "stdafx.h"


//��ʼ��SrvInfo��Ϣ
bool readSrvInfo();
//�滻������Ϣ
bool setSrvInfo();
//����Ŀ���ļ�
bool saveSrvInfoToFile();
bool setInparam();
bool setOutparam();
bool setComFunc();
bool setSrvFunc();
bool setSetCallInfo();
bool setGetCallResult();
bool setGetHDataNodeResult();
bool setParseSrvMsg();
bool setParseComMsg();
bool setParseArrayToNode();
bool setSetErrorXml();


struct SrvInfo 
{
    string IB_SRV_Common_cpp;
    string IB_SRV_Common_h;
    string IB_COM_Common_cpp;
    string IB_COM_Common_h;
    string ParseArrayToNode_cpp;
    string getHDataNodeResult_cpp;
    string ParseArrayToNode_h;
    string getHDataNodeResult_h;

    vector<string> vec_ParseArrayToNode_cpp;
    vector<string> vec_getHDataNodeResult_cpp;
    vector<string> vec_ParseArrayToNode_h;
    vector<string> vec_getHDataNodeResult_h;
};




class CXmlFormat
{
public:
    enum XmlType{ROOT, ARRAY, STRUCT, DATA, ERRNO, ERRINFO};
    CXmlFormat(XmlType type = ROOT):m_type(type) {};

    //xml��������
    XmlType m_type;
};