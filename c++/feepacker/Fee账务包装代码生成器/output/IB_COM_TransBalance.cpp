/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ���ת��
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_TransBalance.h"
#include "view/Server.h"

bool IB_COM_TransBalance::IB_TransBalance( const TransBalanceInfo& data, TransBalanceInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "TransBalance";

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

bool IB_COM_TransBalance::setCallInfo(HRecvMsg *sendmsg, const TransBalanceInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "�ɷ���ˮ��");
    strValue = data.Formnum;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "Դ�ʺ�");
    strValue = data.FromAccoid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "Ŀ���ʺ�");
    strValue = data.ToAccoid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "Ӫҵ��");
    strValue = data.Unit;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "����Ա");
    strValue = data.Op;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ҵ������");
    strValue = data.busicode;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "�ɷѱ������");
    strValue = data.payReason;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_TransBalance::getCallResult(HSendMsg *rcvmsg, TransBalanceInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "������");
    if( !StrToCstr(res.ErrNo, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[������]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "ת�ƽ��");
    if( !StrToCstr(res.TransFee, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[ת�ƽ��]" ) )
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






