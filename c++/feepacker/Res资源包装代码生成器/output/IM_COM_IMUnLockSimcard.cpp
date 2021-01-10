/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   SIM�������ͷ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IM_COM_IMUnLockSimcard.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <vector>


bool IM_COM_IMUnLockSimcard::IM_IMUnLockSimcard( const IMUnLockSimcardInfo& data, IMUnLockSimcardInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IMUnLockSimcard";

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

bool IM_COM_IMUnLockSimcard::setCallInfo(HRecvMsg *sendmsg, const IMUnLockSimcardInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.Region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��λ����");
    strValue = data.OrgID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "����Ա����");
    strValue = data.OperID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "SIM����");
    strValue = data.ICCID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "��������");
    strValue = data.OprType;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ҵ��������ˮ��");
    strValue = data.RecOID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "ʵ�����ۼ۸�");
    strValue = data.Price;
    node->AddSimpleData(strValue);


    return true;
}


bool IM_COM_IMUnLockSimcard::getCallResult(HSendMsg *rcvmsg, IMUnLockSimcardInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "�������");
    if( !StrToCstr(res.RetCode, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[�������]" ) )
    {
        return false;
    }

    //���ش�����Ϣ
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrMsg, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}






