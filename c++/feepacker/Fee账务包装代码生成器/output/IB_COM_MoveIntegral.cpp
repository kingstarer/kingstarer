/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ����ת��
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_MoveIntegral.h"
#include "view/Server.h"

bool IB_COM_MoveIntegral::IB_MoveIntegral( const MoveIntegralInfo& data, MoveIntegralInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "MoveIntegral";

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

bool IB_COM_MoveIntegral::setCallInfo(HRecvMsg *sendmsg, const MoveIntegralInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "ҵ����ˮ��");
    strValue = data.Businum;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ת���ͻ�����");
    strValue = data.Outcustid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ת���������");
    strValue = data.OutServnumber;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ת����Ʒ����");
    strValue = data.OutProdtype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.Region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ת��ͻ�����");
    strValue = data.Incustid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ת��������");
    strValue = data.InServnumber;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ת���Ʒ����");
    strValue = data.InProdtype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "����Ա");
    strValue = data.Oprcode;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "���ֱ��ԭ��");
    strValue = data.Intchgrsn;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ת�ƻ���");
    strValue = data.Moveintegral;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.Movecycle;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "Ӫҵ��");
    strValue = data.Unit;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_MoveIntegral::getCallResult(HSendMsg *rcvmsg, MoveIntegralInfoOut& res)
{
    LogHere();

    /*���ز�������*/
    HDataNode *node = rcvmsg->GetRootNode();
    //�������ز�������
    const int expectCnt = 2;

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

    //���ش�����Ϣ
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrInfo, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}






