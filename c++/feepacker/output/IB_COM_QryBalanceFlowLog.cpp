/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ���������ʷ����Ŀ�����־����ѯ�ӿ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_QryBalanceFlowLog.h"
#include "view/Server.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <vector>


bool IB_COM_QryBalanceFlowLog::IB_QryBalanceFlowLog( const QryBalanceFlowLogInfo& data, QryBalanceFlowLogInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "QryBalanceFlowLog";

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

bool IB_COM_QryBalanceFlowLog::setCallInfo(HRecvMsg *sendmsg, const QryBalanceFlowLogInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
	//�ʺ�
	strValue = data.Accoid;
	node->AddSimpleData(strValue);

	//��ʼʱ��
	strValue = data.StartTime;
	node->AddSimpleData(strValue);

	//����ʱ��
	strValue = data.EndTime;
	node->AddSimpleData(strValue);

	//��ѯ����
	strValue = data.qryType;
	node->AddSimpleData(strValue);



    return true;
}


bool IB_COM_QryBalanceFlowLog::getCallResult(HSendMsg *rcvmsg, QryBalanceFlowLogInfoOut& res)
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

	if( !getHDataNodeResult(res.ALogArray, node->DataList[pos++], 
			"����ṹ��[������Ϣ����]" ) )
	{
		return false;
	}

	if( StrToCstr(res.ErrNo, *(node->DataList[pos++]->GetKey()), 
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


bool IB_COM_QryBalanceFlowLog::getHDataNodeResult(vector<ALog>& nodeinfo, IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 12;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        ALog item;
        string memname(destname);

		if( !StrToCstr(item.SubjectId, *(node->DataList[pos++]->GetKey()), 
			memname + "[��Ŀ����Ŀ������]") )
		{
			return false;
		}

		if( !StrToCstr(item.StartCycle, *(node->DataList[pos++]->GetKey()), 
			memname + "[��Ŀ��ʼ����]") )
		{
			return false;
		}

		if( !StrToCstr(item.EndCycle, *(node->DataList[pos++]->GetKey()), 
			memname + "[��Ŀ��������]") )
		{
			return false;
		}

		if( !StrToCstr(item.FlowTime, *(node->DataList[pos++]->GetKey()), 
			memname + "[����ʱ��]") )
		{
			return false;
		}

		if( !StrToCstr(item.FlowDirection, *(node->DataList[pos++]->GetKey()), 
			memname + "[��������]") )
		{
			return false;
		}

		if( !StrToCstr(item.FlowType, *(node->DataList[pos++]->GetKey()), 
			memname + "[��������]") )
		{
			return false;
		}

		if( !StrToCstr(item.FlowTypeName, *(node->DataList[pos++]->GetKey()), 
			memname + "[���ԭ��]") )
		{
			return false;
		}

		if( !StrToCstr(item.FlowAmt, *(node->DataList[pos++]->GetKey()), 
			memname + "[�������]") )
		{
			return false;
		}

		if( !StrToCstr(item.FlowNoBalance, *(node->DataList[pos++]->GetKey()), 
			memname + "[������ʣ��]") )
		{
			return false;
		}

		if( !StrToCstr(item.OperID, *(node->DataList[pos++]->GetKey()), 
			memname + "[�����˹���]") )
		{
			return false;
		}

		if( !StrToCstr(item.Formnum, *(node->DataList[pos++]->GetKey()), 
			memname + "[��ˮ��]") )
		{
			return false;
		}

		if( !StrToCstr(item.Source, *(node->DataList[pos++]->GetKey()), 
			memname + "[��Դ]") )
		{
			return false;
		}


        nodeinfo.push_back(item);
    }

    return true;
}


