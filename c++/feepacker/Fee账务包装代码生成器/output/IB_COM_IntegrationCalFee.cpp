/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ��������ѽӿ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_IntegrationCalFee.h"
#include "view/Server.h"

bool IB_COM_IntegrationCalFee::IB_IntegrationCalFee( const IntegrationCalFeeInfo& data, IntegrationCalFeeInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IntegrationCalFee";

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

bool IB_COM_IntegrationCalFee::setCallInfo(HRecvMsg *sendmsg, const IntegrationCalFeeInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "�û����");
    strValue = data.subsid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ҵ���ʶ");
    strValue = data.recflag;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��Ʒ�����б�");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.ProdListArray) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "Ӫ�����������б�");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.YxPlanListArray) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "ҵ������");
    strValue = data.TradeType;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "����ʱ��");
    strValue = data.ActiveTime;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_IntegrationCalFee::getCallResult(HSendMsg *rcvmsg, IntegrationCalFeeInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "��ѽ���б�");
    if( !getHDataNodeResult(res.CalFeeResultListArray, node->DataList[pos++], 
            "����ṹ��[��ѽ���б�]" ) )
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


bool IB_COM_IntegrationCalFee::getHDataNodeResult(vector<ProdList>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 3;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        ProdList item;
        string memname(destname);

        if( !StrToCstr(item.ProdId, *(node->DataList[pos++]->GetKey()), 
            memname + "[��Ʒ���]") )
        {
            return false;
        }

        if( !StrToCstr(item.StratTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[��Чʱ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.StopTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[ʧЧʱ��]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_IntegrationCalFee::getHDataNodeResult(vector<YxPlanList>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 3;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        YxPlanList item;
        string memname(destname);

        if( !StrToCstr(item.YxPlanId, *(node->DataList[pos++]->GetKey()), 
            memname + "[Ӫ���������]") )
        {
            return false;
        }

        if( !StrToCstr(item.StratTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[��Чʱ��]") )
        {
            return false;
        }

        if( !StrToCstr(item.StopTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[ʧЧʱ��]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_IntegrationCalFee::getHDataNodeResult(vector<CalFeeResultList>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 3;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        CalFeeResultList item;
        string memname(destname);

        if( !StrToCstr(item.acctid, *(node->DataList[pos++]->GetKey()), 
            memname + "[�ʵ���Ŀ����]") )
        {
            return false;
        }

        if( !StrToCstr(item.recamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[ʵ�ս��]") )
        {
            return false;
        }

        if( !StrToCstr(item.subamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[������]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_IntegrationCalFee::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<ProdList>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //��Ʒ���
        strValue = nodeinfo[i].ProdId;
        node->AddSimpleData(strValue);

        //��Чʱ��
        strValue = nodeinfo[i].StratTime;
        node->AddSimpleData(strValue);

        //ʧЧʱ��
        strValue = nodeinfo[i].StopTime;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_IntegrationCalFee::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<YxPlanList>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //Ӫ���������
        strValue = nodeinfo[i].YxPlanId;
        node->AddSimpleData(strValue);

        //��Чʱ��
        strValue = nodeinfo[i].StratTime;
        node->AddSimpleData(strValue);

        //ʧЧʱ��
        strValue = nodeinfo[i].StopTime;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_IntegrationCalFee::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<CalFeeResultList>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //�ʵ���Ŀ����
        strValue = nodeinfo[i].acctid;
        node->AddSimpleData(strValue);

        //ʵ�ս��
        strValue = nodeinfo[i].recamt;
        node->AddSimpleData(strValue);

        //������
        strValue = nodeinfo[i].subamt;
        node->AddSimpleData(strValue);


    }

    return true;
}



