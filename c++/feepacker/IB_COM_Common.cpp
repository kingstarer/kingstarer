/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   <�������>
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_<Common>.h"
#include "view/Server.h"


bool IB_COM_<Common>::IB_<Common>( const <inparam>& data, <outparam>& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "<Common>";

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
        BASE_LOG_ERROR("ת��ʧ��");
        return false;
    }
    

    BASE_LOG_INFO("����Զ�̽ӿ�");
    if ( !CallRemoteCics(sendmsg.get(), rcvmsg.get(), res) )
    {
        BASE_LOG_ERROR("����ʧ��");
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

bool IB_COM_<Common>::setCallInfo(HRecvMsg *sendmsg, const <inparam>& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
<setCallInfo>
    return true;
}


bool IB_COM_<Common>::getCallResult(HSendMsg *rcvmsg, <outparam>& res)
{
    LogHere();

    /*���ز�������*/
    HDataNode *node = rcvmsg->GetRootNode();
    //�������ز�������
    const int expectCnt = <getCallResultCnt>;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

<getCallResult>
    m_iErrorCode = 1;
    return true;
}


<getHDataNodeResultDefineImplement>

<ParseArrayToNodeDefineImplement>

