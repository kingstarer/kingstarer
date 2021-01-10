/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ��ͻ�������Ϣ
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_QryServIntegral.h"
#include "view/Server.h"

bool IB_COM_QryServIntegral::IB_QryServIntegral( const QryServIntegralInfo& data, QryServIntegralInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "QryServIntegral";

    //Msg Define
    auto_ptr<HRecvMsg> sendmsg ( new HRecvMsg() );              // ��������
    auto_ptr<HSendMsg> rcvmsg  ( new HSendMsg() );              // ��������


    BASE_LOG_INFO("���ṹ��ת����xml����");

    if ( setCallInfo(sendmsg.get(), data) )
    {
        BASE_LOG_INFO("ת���ɹ�");
    }
    else
    {
         BASE_LOG_INFO("ת��ʧ��");
         return false;
    }
    

    BASE_LOG_INFO("����Զ�̽ӿ�");
    if ( !CallRemoteCics(sendmsg.get(), rcvmsg.get(), res) )
    {
        BASE_LOG_INFO("����ʧ��");
        return false;
    }

    BASE_LOG_INFO("����cics���ر���");
    if (!getCallResult(rcvmsg.get(), res))
    {
        BASE_LOG_ERROR("����cics���ر���ʧ��");
        return false;
    }
    

    BASE_LOG_DEBUG("���������:%d,������Ϣ:%s", m_iErrorCode, m_sErrorStr.c_str());
    BASE_LOG_INFO( "-------------ִ�е���CICS�ӿ�END:%s-------------- ", m_rpcname );

    return true;
}

bool IB_COM_QryServIntegral::setCallInfo(HRecvMsg *sendmsg, const QryServIntegralInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.Region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�������");
    strValue = data.Servnumber;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�ͻ����");
    strValue = data.CustID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��Ʒ����");
    strValue = data.Prodtype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.Integralcyc;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ȫƷ�ƻ�������");
    strValue = data.Brandscoretype;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_QryServIntegral::getCallResult(HSendMsg *rcvmsg, QryServIntegralInfoOut& res)
{
    LogHere();

    /*���ز�������*/
    HDataNode *node = rcvmsg->GetRootNode();
    //�������ز�������
    const int expectCnt = 3;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

    BASE_LOG_DEBUG("%s", "������Ϣ����");
    if( !getHDataNodeResult(res.ACustIntegralArray, node->DataList[pos++], 
            "����ṹ��[������Ϣ����]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "������");
    if( !StrToCstr(res.ErrNo, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[������]" ) )
    {
        return false;
    }

    //���ش�����Ϣ
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrInfo, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}


bool IB_COM_QryServIntegral::getHDataNodeResult(vector<ACustIntegral>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 10;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        ACustIntegral item;
        string memname(destname);

        if( !StrToCstr(item.ServNumber, *(node->DataList[pos++]->GetKey()), 
            memname + "[�������]") )
        {
            return false;
        }

        if( !StrToCstr(item.Totalintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[�ܻ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.Integralcyc, *(node->DataList[pos++]->GetKey()), 
            memname + "[��������]") )
        {
            return false;
        }

        if( !StrToCstr(item.Availintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[���û���]") )
        {
            return false;
        }

        if( !StrToCstr(item.Moveintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[ת�ƻ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.Unavailintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[ʧЧ����]") )
        {
            return false;
        }

        if( !StrToCstr(item.Saleintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[�������]") )
        {
            return false;
        }

        if( !StrToCstr(item.Updatetime, *(node->DataList[pos++]->GetKey()), 
            memname + "[���¸���ʱ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.Stoptime, *(node->DataList[pos++]->GetKey()), 
            memname + "[ʧЧʱ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.Brandscoretype, *(node->DataList[pos++]->GetKey()), 
            memname + "[ȫƷ�ƻ�������]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_QryServIntegral::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<ACustIntegral>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //�������
        strValue = nodeinfo[i].ServNumber;
        node->AddSimpleData(strValue);

        //�ܻ���
        strValue = nodeinfo[i].Totalintegral;
        node->AddSimpleData(strValue);

        //��������
        strValue = nodeinfo[i].Integralcyc;
        node->AddSimpleData(strValue);

        //���û���
        strValue = nodeinfo[i].Availintegral;
        node->AddSimpleData(strValue);

        //ת�ƻ���
        strValue = nodeinfo[i].Moveintegral;
        node->AddSimpleData(strValue);

        //ʧЧ����
        strValue = nodeinfo[i].Unavailintegral;
        node->AddSimpleData(strValue);

        //�������
        strValue = nodeinfo[i].Saleintegral;
        node->AddSimpleData(strValue);

        //���¸���ʱ��
        strValue = nodeinfo[i].Updatetime;
        node->AddSimpleData(strValue);

        //ʧЧʱ��
        strValue = nodeinfo[i].Stoptime;
        node->AddSimpleData(strValue);

        //ȫƷ�ƻ�������
        strValue = nodeinfo[i].Brandscoretype;
        node->AddSimpleData(strValue);


    }

    return true;
}



